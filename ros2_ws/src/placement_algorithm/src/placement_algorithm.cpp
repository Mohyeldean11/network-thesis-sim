#include "./../include/placement_algorithm.hpp"
Placement_Algorithm::Placement_Algorithm(const placement_config &config): configOBJ(config)
{
    if (!std::isfinite(configOBJ.local_processing_ms) || configOBJ.local_processing_ms <= 0.0)
    {
        throw std::invalid_argument(
            "Local processing time must be finite and greater than zero");
    }

    if (!std::isfinite(configOBJ.edge_processing_ms) || configOBJ.edge_processing_ms <= 0.0)
    {
        throw std::invalid_argument(
            "Edge processing time must be finite and greater than zero");
    }

    if (!std::isfinite(configOBJ.max_RTT_MS) || configOBJ.max_RTT_MS <= 0.0)
    {
        throw std::invalid_argument(
            "Maximum RTT must be finite and greater than zero");
    }

    if (!std::isfinite(configOBJ.min_bandwidth_mbps) || configOBJ.min_bandwidth_mbps <= 0.0)
    {
        throw std::invalid_argument(
            "Minimum bandwidth must be finite and greater than zero");
    }

    if (!std::isfinite(configOBJ.max_packet_loss) || configOBJ.max_packet_loss < 0.0 || configOBJ.max_packet_loss > 100.0)
    {
        throw std::invalid_argument(
            "Maximum packet loss must be between 0 and 100 percent");
    }
}
placement_decision Placement_Algorithm::Decide(const Network_Metrics &Metrics_config, std::size_t image_size_bytes) const
{
    placement_decision result_OBJ_Decision;
    bool failed_network_constraints = false;
    result_OBJ_Decision.estimated_local_ms = configOBJ.local_processing_ms;
                                                                        /*VALIDATING THE IMAGE CONDITIONS*/
    if (image_size_bytes == 0)
    {
        result_OBJ_Decision.Reason = "Invalid image: image size is zero";
        return result_OBJ_Decision;
    }
                                    
                                                                        /*VALIDATING THE NETWORK CONDITIONS*/
    if (!std::isfinite(Metrics_config.RTT) || Metrics_config.RTT < 0.0)
    {
        result_OBJ_Decision.Reason = "Invalid RTT measurement";

        return result_OBJ_Decision;
    }

    if (!std::isfinite(Metrics_config.bandwidths_mbps) || Metrics_config.bandwidths_mbps <= 0.0)
    {
        result_OBJ_Decision.Reason ="Invalid bandwidth measurement";

        return result_OBJ_Decision;
    }

    if (!std::isfinite(Metrics_config.packet_loss_percentage) || Metrics_config.packet_loss_percentage < 0.0 || Metrics_config.packet_loss_percentage > 100.0)
    {
        result_OBJ_Decision.Reason = "Invalid packet loss measurement";

        return result_OBJ_Decision;
    }

    //checking threshold value

    if(configOBJ.min_bandwidth_mbps > Metrics_config.bandwidths_mbps)
    {
        result_OBJ_Decision.Reason += "the bandwidths_mbps " + std::to_string(Metrics_config.bandwidths_mbps) +"is less than the min threshold" + std::to_string(configOBJ.min_bandwidth_mbps) + "\n";
        failed_network_constraints = true;
    }
    if(configOBJ.max_RTT_MS < Metrics_config.RTT)
    {
        result_OBJ_Decision.Reason += "the RTT " + std::to_string(Metrics_config.RTT) +"is Higher than the max threshold" + std::to_string(configOBJ.max_RTT_MS) + "\n";
        failed_network_constraints = true;

    }
    if(configOBJ.max_packet_loss < Metrics_config.packet_loss_percentage)
    {
        result_OBJ_Decision.Reason += "the packet loss " + std::to_string(Metrics_config.packet_loss_percentage) +"is Higher than the max threshold" + std::to_string(configOBJ.max_packet_loss) + "\n";
        failed_network_constraints = true;

    }

    if (failed_network_constraints)
    {
        return result_OBJ_Decision;
    }

    result_OBJ_Decision.Transmission_ms = estimate_transmission_time(image_size_bytes , Metrics_config.bandwidths_mbps);
    //T_edge = RTT + T_transmission + T_edge_processing
    result_OBJ_Decision.estimated_edge_ms = result_OBJ_Decision.Transmission_ms + Metrics_config.RTT + configOBJ.edge_processing_ms;

    if(result_OBJ_Decision.estimated_edge_ms <= result_OBJ_Decision.estimated_local_ms)
    {
        result_OBJ_Decision.Decision_target =ExecutionTarget::EDGE_COMPUTER;
        result_OBJ_Decision.Reason += "the estimated edge time " + std::to_string(result_OBJ_Decision.estimated_edge_ms) +"is lower than the estimated Local time" + std::to_string(result_OBJ_Decision.estimated_local_ms) + "\n";

    }
    else
    {
        result_OBJ_Decision.Decision_target = ExecutionTarget::LOCAL_COMPUTER;
        result_OBJ_Decision.Reason += "the estimated edge time " + std::to_string(result_OBJ_Decision.estimated_edge_ms) +"is higher than the estimated Local time" + std::to_string(result_OBJ_Decision.estimated_local_ms) + "\n";

    }

    return result_OBJ_Decision;
}
double Placement_Algorithm::estimate_transmission_time(std::size_t image_size_bytes, double bandwidth_mbps) const
{
    //T_transmission = ((ImageSizeBytes X 8) / (BandwidthMbps X 10^6)) X 1000
    double transmissiontime{((static_cast<double>(image_size_bytes) * 8) / (bandwidth_mbps * 1000000.0)) * 1000};
    return transmissiontime;
}