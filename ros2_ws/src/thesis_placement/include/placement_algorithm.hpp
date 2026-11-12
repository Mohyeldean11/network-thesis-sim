#ifndef THESIS_PLACEMENT_ALGORITHM_HPP_
#define THESIS_PLACEMENT_ALGORITHM_HPP_
#include <cstddef>
#include <string>
#include <cmath>
#include <stdexcept>

enum class ExecutionTarget {LOCAL_COMPUTER=1 , EDGE_COMPUTER};

struct Network_Metrics
{
    /* Network Metrics configuration */
    double RTT{10.0} ;
    double bandwidths_mbps{100.0};
    double packet_loss_percentage{0.0} ;
};

struct placement_config
{
    /* Input config Data */
    double local_processing_ms {10.0};
    double edge_processing_ms {10.0};
    double max_RTT_MS {50.0};
    double min_bandwidth_mbps {1.0};
    double max_packet_loss{2.0};
};
struct placement_decision
{
    /*Output Data*/
    double estimated_local_ms{0};
    double estimated_edge_ms{0};
    double Transmission_ms{0};
    std::string Reason{};
    ExecutionTarget Decision_target{ExecutionTarget::LOCAL_COMPUTER};
};

class Placement_Algorithm
{
private:
    placement_config configOBJ;

public:
    Placement_Algorithm(const placement_config &config);
    ~Placement_Algorithm() =default;
    placement_decision Decide(const Network_Metrics &Metrics_config , std::size_t image_size_bytes) const;
    double estimate_transmission_time(std::size_t image_size_bytes, double bandwidth_mbps) const;
};


#endif