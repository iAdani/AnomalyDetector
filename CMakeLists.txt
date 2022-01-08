cmake_minimum_required(VERSION 3.21)
project(Mitkadem1Ex6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Mitkadem1Ex6
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        AnomalyDetector.h
        CLI.cpp
        CLI.h
        commands.h
        HybridAnomalyDetector.cpp
        HybridAnomalyDetector.h
        MainTrain.cpp
        minCircle.cpp
        minCircle.h
        Server.cpp
        Server.h
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        timeseries.cpp
        timeseries.h)
