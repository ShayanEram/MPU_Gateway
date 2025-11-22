#include "HAL.hpp"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <cstring>

#include <open62541/client.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>

#include <modbus/modbus.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <fstream>
#include <sstream>

ModbusRTUClient::ModbusRTUClient(const std::string& device, int baudrate)
    : fd_(-1), device_(device), baudrate_(baudrate) {}

ModbusRTUClient::~ModbusRTUClient() {
    disconnect();
}

bool ModbusRTUClient::connect() {
    fd_ = open(device_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_ == -1) {
        perror("open");
        return false;
    }

    struct termios options{};
    tcgetattr(fd_, &options);

    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;     // 8 data bits
    options.c_cflag |= (CLOCAL | CREAD);

    tcsetattr(fd_, TCSANOW, &options);
    return true;
}

void ModbusRTUClient::disconnect() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

uint16_t ModbusRTUClient::crc16(const uint8_t* buf, int len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

bool ModbusRTUClient::sendRequest(const std::vector<uint8_t>& req) {
    if (fd_ == -1) return false;
    int written = write(fd_, req.data(), req.size());
    return written == (int)req.size();
}

bool ModbusRTUClient::receiveResponse(std::vector<uint8_t>& resp) {
    if (fd_ == -1) return false;
    uint8_t buf[256];
    int n = read(fd_, buf, sizeof(buf));
    if (n <= 0) return false;
    resp.assign(buf, buf + n);
    return true;
}

bool ModbusRTUClient::readHoldingRegisters(uint8_t slaveAddr, uint16_t startAddr,
                                           uint16_t count, std::vector<uint16_t>& outRegs) {
    std::vector<uint8_t> req;
    req.push_back(slaveAddr);
    req.push_back(0x03); // Function code: Read Holding Registers
    req.push_back(startAddr >> 8);
    req.push_back(startAddr & 0xFF);
    req.push_back(count >> 8);
    req.push_back(count & 0xFF);

    uint16_t crc = crc16(req.data(), req.size());
    req.push_back(crc & 0xFF);
    req.push_back(crc >> 8);

    if (!sendRequest(req)) return false;

    std::vector<uint8_t> resp;
    if (!receiveResponse(resp)) return false;

    // Validate CRC
    if (resp.size() < 5) return false;
    uint16_t respCrc = (resp[resp.size()-1] << 8) | resp[resp.size()-2];
    if (crc16(resp.data(), resp.size()-2) != respCrc) return false;

    // Parse data
    uint8_t byteCount = resp[2];
    if (resp.size() != (5 + byteCount - 1)) return false;

    outRegs.clear();
    for (int i = 0; i < byteCount/2; i++) {
        uint16_t val = (resp[3 + 2*i] << 8) | resp[4 + 2*i];
        outRegs.push_back(val);
    }
    return true;
}

I2CDevice::I2CDevice(const std::string& device, uint8_t address)
    : fd_(-1), device_(device), address_(address) {}

I2CDevice::~I2CDevice() {
    closeBus();
}

bool I2CDevice::openBus() {
    fd_ = open(device_.c_str(), O_RDWR);
    if (fd_ < 0) {
        perror("open");
        return false;
    }
    if (ioctl(fd_, I2C_SLAVE, address_) < 0) {
        perror("ioctl");
        close(fd_);
        fd_ = -1;
        return false;
    }
    return true;
}

void I2CDevice::closeBus() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

bool I2CDevice::writeByte(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    if (write(fd_, buf, 2) != 2) {
        perror("writeByte");
        return false;
    }
    return true;
}

bool I2CDevice::writeBytes(uint8_t reg, const std::vector<uint8_t>& values) {
    std::vector<uint8_t> buf;
    buf.push_back(reg);
    buf.insert(buf.end(), values.begin(), values.end());
    if (write(fd_, buf.data(), buf.size()) != (int)buf.size()) {
        perror("writeBytes");
        return false;
    }
    return true;
}

bool I2CDevice::readByte(uint8_t reg, uint8_t& value) {
    if (write(fd_, &reg, 1) != 1) {
        perror("readByte: write reg");
        return false;
    }
    if (read(fd_, &value, 1) != 1) {
        perror("readByte: read val");
        return false;
    }
    return true;
}

bool I2CDevice::readBytes(uint8_t reg, size_t count, std::vector<uint8_t>& values) {
    if (write(fd_, &reg, 1) != 1) {
        perror("readBytes: write reg");
        return false;
    }
    values.resize(count);
    if (read(fd_, values.data(), count) != (int)count) {
        perror("readBytes: read vals");
        return false;
    }
    return true;
}

SPIDevice::SPIDevice(const std::string& device, uint32_t speedHz, uint8_t mode, uint8_t bitsPerWord)
    : fd_(-1), device_(device), speedHz_(speedHz), mode_(mode), bitsPerWord_(bitsPerWord) {}

SPIDevice::~SPIDevice() {
    closeBus();
}

bool SPIDevice::openBus() {
    fd_ = open(device_.c_str(), O_RDWR);
    if (fd_ < 0) {
        perror("open");
        return false;
    }

    if (ioctl(fd_, SPI_IOC_WR_MODE, &mode_) < 0) {
        perror("SPI_IOC_WR_MODE");
        return false;
    }
    if (ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord_) < 0) {
        perror("SPI_IOC_WR_BITS_PER_WORD");
        return false;
    }
    if (ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speedHz_) < 0) {
        perror("SPI_IOC_WR_MAX_SPEED_HZ");
        return false;
    }
    return true;
}

void SPIDevice::closeBus() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

bool SPIDevice::transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx) {
    if (fd_ < 0) return false;
    rx.resize(tx.size());

    struct spi_ioc_transfer tr{};
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)tx.data();
    tr.rx_buf = (unsigned long)rx.data();
    tr.len = tx.size();
    tr.speed_hz = speedHz_;
    tr.bits_per_word = bitsPerWord_;

    if (ioctl(fd_, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SPI transfer");
        return false;
    }
    return true;
}

bool SPIDevice::writeBytes(const std::vector<uint8_t>& tx) {
    std::vector<uint8_t> dummy;
    return transfer(tx, dummy);
}

bool SPIDevice::readBytes(size_t count, std::vector<uint8_t>& rx) {
    std::vector<uint8_t> tx(count, 0x00); // send dummy bytes
    return transfer(tx, rx);
}

OPCUAClient::OPCUAClient(const std::string& endpointUrl)
    : endpointUrl_(endpointUrl), client_(nullptr) {}

OPCUAClient::~OPCUAClient() {
    disconnect();
}

bool OPCUAClient::connect() {
    client_ = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig((UA_Client*)client_));

    UA_StatusCode retval = UA_Client_connect((UA_Client*)client_, endpointUrl_.c_str());
    if (retval != UA_STATUSCODE_GOOD) {
        std::cerr << "OPC UA connect failed: " << UA_StatusCode_name(retval) << "\n";
        UA_Client_delete((UA_Client*)client_);
        client_ = nullptr;
        return false;
    }
    return true;
}

void OPCUAClient::disconnect() {
    if (client_) {
        UA_Client_disconnect((UA_Client*)client_);
        UA_Client_delete((UA_Client*)client_);
        client_ = nullptr;
    }
}

bool OPCUAClient::readValue(const std::string& nodeId, double& outValue) {
    if (!client_) return false;

    UA_Variant value;
    UA_Variant_init(&value);

    UA_NodeId node = UA_NODEID_STRING_ALLOC(1, nodeId.c_str());
    UA_StatusCode retval = UA_Client_readValueAttribute((UA_Client*)client_, node, &value);
    UA_NodeId_clear(&node);

    if (retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DOUBLE])) {
        outValue = *(double*)value.data;
        UA_Variant_clear(&value);
        return true;
    }
    UA_Variant_clear(&value);
    return false;
}

bool OPCUAClient::writeValue(const std::string& nodeId, double value) {
    if (!client_) return false;

    UA_Variant v;
    UA_Variant_setScalar(&v, &value, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_NodeId node = UA_NODEID_STRING_ALLOC(1, nodeId.c_str());
    UA_StatusCode retval = UA_Client_writeValueAttribute((UA_Client*)client_, node, &v);
    UA_NodeId_clear(&node);

    return retval == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::publishSensorData(const std::string& nodeId, const std::vector<double>& values) {
    bool ok = true;
    for (size_t i = 0; i < values.size(); i++) {
        ok &= writeValue(nodeId, values[i]);
    }
    return ok;
}

ModbusTCPClient::ModbusTCPClient(const std::string& ip, int port)
    : ip_(ip), port_(port), ctx_(nullptr) {}

ModbusTCPClient::~ModbusTCPClient() {
    disconnect();
}

bool ModbusTCPClient::connect() {
    ctx_ = modbus_new_tcp(ip_.c_str(), port_);
    if (!ctx_) {
        std::cerr << "Unable to create Modbus TCP context\n";
        return false;
    }
    if (modbus_connect((modbus_t*)ctx_) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        modbus_free((modbus_t*)ctx_);
        ctx_ = nullptr;
        return false;
    }
    return true;
}

void ModbusTCPClient::disconnect() {
    if (ctx_) {
        modbus_close((modbus_t*)ctx_);
        modbus_free((modbus_t*)ctx_);
        ctx_ = nullptr;
    }
}

bool ModbusTCPClient::readHoldingRegisters(int addr, int nb, std::vector<uint16_t>& outRegs) {
    if (!ctx_) return false;
    outRegs.resize(nb);
    int rc = modbus_read_registers((modbus_t*)ctx_, addr, nb, outRegs.data());
    return rc == nb;
}

bool ModbusTCPClient::writeRegister(int addr, uint16_t value) {
    if (!ctx_) return false;
    int rc = modbus_write_register((modbus_t*)ctx_, addr, value);
    return rc == 1;
}

bool ModbusTCPClient::writeRegisters(int addr, const std::vector<uint16_t>& values) {
    if (!ctx_) return false;
    int rc = modbus_write_registers((modbus_t*)ctx_, addr, values.size(), values.data());
    return rc == (int)values.size();
}

CANDevice::CANDevice(const std::string& ifname)
    : ifname_(ifname), fd_(-1) {}

CANDevice::~CANDevice() {
    closeBus();
}

bool CANDevice::openBus() {
    fd_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (fd_ < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr{};
    std::strncpy(ifr.ifr_name, ifname_.c_str(), IFNAMSIZ - 1);
    if (ioctl(fd_, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(fd_);
        fd_ = -1;
        return false;
    }

    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd_);
        fd_ = -1;
        return false;
    }

    return true;
}

void CANDevice::closeBus() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

bool CANDevice::sendFrame(const CANFrame& frame) {
    if (fd_ < 0) return false;

    struct can_frame cf{};
    cf.can_id = frame.id;
    cf.can_dlc = frame.dlc;
    std::memcpy(cf.data, frame.data, frame.dlc);

    int nbytes = write(fd_, &cf, sizeof(cf));
    return nbytes == sizeof(cf);
}

bool CANDevice::receiveFrame(CANFrame& frame) {
    if (fd_ < 0) return false;

    struct can_frame cf{};
    int nbytes = read(fd_, &cf, sizeof(cf));
    if (nbytes < 0) {
        perror("read");
        return false;
    }
    if (nbytes < (int)sizeof(struct can_frame)) {
        std::cerr << "Incomplete CAN frame\n";
        return false;
    }

    frame.id = cf.can_id;
    frame.dlc = cf.can_dlc;
    std::memcpy(frame.data, cf.data, cf.can_dlc);
    return true;
}

PWMChannel::PWMChannel(int chip, int channel)
    : chip_(chip), channel_(channel) {
    std::ostringstream oss;
    oss << "/sys/class/pwm/pwmchip" << chip_;
    basePath_ = oss.str();
}

PWMChannel::~PWMChannel() {
    disable();
    unexportChannel();
}

bool PWMChannel::exportChannel() {
    std::ofstream f(basePath_ + "/export");
    if (!f.is_open()) return false;
    f << channel_;
    return true;
}

void PWMChannel::unexportChannel() {
    std::ofstream f(basePath_ + "/unexport");
    if (f.is_open()) f << channel_;
}

bool PWMChannel::enable() {
    std::ofstream f(basePath_ + "/pwm" + std::to_string(channel_) + "/enable");
    if (!f.is_open()) return false;
    f << 1;
    return true;
}

void PWMChannel::disable() {
    std::ofstream f(basePath_ + "/pwm" + std::to_string(channel_) + "/enable");
    if (f.is_open()) f << 0;
}

bool PWMChannel::setPeriod(unsigned int nanoseconds) {
    std::ofstream f(basePath_ + "/pwm" + std::to_string(channel_) + "/period");
    if (!f.is_open()) return false;
    f << nanoseconds;
    return true;
}

bool PWMChannel::setDutyCycle(unsigned int nanoseconds) {
    std::ofstream f(basePath_ + "/pwm" + std::to_string(channel_) + "/duty_cycle");
    if (!f.is_open()) return false;
    f << nanoseconds;
    return true;
}