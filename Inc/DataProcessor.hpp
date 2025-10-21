#ifndef INC_DATAPROCESSOR_HPP
#define INC_DATAPROCESSOR_HPP

class DataProcessor {
public:
    DataProcessor() = default;
    ~DataProcessor() = default;

    void start();
    void stop();
    void loop();
};

#endif // INC_DATAPROCESSOR_HPP