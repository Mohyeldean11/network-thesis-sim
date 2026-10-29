FROM ros:jazzy-ros-base-noble

ENV DEBIAN_FRONTEND=noninteractive

#installing the development and measurement tools for reuse
RUN apt-get update && apt-get install -y --no-install-recommends \
	python3-pip \
	python3-colcon-common-extensions \
	python3-rosdep \
	python3-vcstool \
	git \
	build-essential \
	&& rm -rf /var/lib/apt/lists/*


#creating a workdirectory for ROS 2
WORKDIR /workspace

#sourcing ROS2 into interactive bash 
RUN echo "source /opt/ros/jazzy/setup.bash" >> /root/.bashrc

CMD ["bash"]
