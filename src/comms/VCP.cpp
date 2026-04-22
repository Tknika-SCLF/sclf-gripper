#include "comms/VCP.h"

VCPManager* VCPManager::_instance = nullptr;

VCPManager::VCPManager(Stream& serial) 
    : _serial(serial), _commander(serial) {
    _instance = this;
}

void VCPManager::begin() {
    // Habilitar debug de SimpleFOC
    SimpleFOCDebug::enable(&_serial);
    
    // Configurar Commander
    _commander.verbose = VerboseMode::user_friendly;
    
    // Añadir comando para control de telemetría propio
    _commander.add('T', _onTelemetry, "telemetry (1:ON, 0:OFF)");
}

void VCPManager::registerMotor(MotorController& mc) {
    _mc = &mc;
    // Registrar el motor bajo el comando 'M'
    _commander.add('M', _onMotor, "motor");
}

void VCPManager::update() {
    // Procesar comandos entrantes
    _commander.run();

    // Gestionar telemetría si está activada (cada 100ms por defecto)
    if (_telemetryEnabled && _mc) {
        if (millis() - _lastTelemetryTime > 100) {
            BLDCMotor& motor = _mc->getMotor();
            _serial.print("V:"); _serial.print(motor.shaft_velocity, 2);
            _serial.print(" A:"); _serial.print(motor.shaft_angle, 3);
            _serial.print(" T:"); _serial.println(motor.target, 2);
            _lastTelemetryTime = millis();
        }
    }
}

// Callbacks del Commander
void VCPManager::_onMotor(char* cmd) {
    if (_instance && _instance->_mc) {
        _instance->_commander.motor(&_instance->_mc->getMotor(), cmd);
    }
}

void VCPManager::_onTelemetry(char* cmd) {
    if (_instance) {
        int val = atoi(cmd);
        _instance->setTelemetry(val > 0);
        _instance->_serial.print("Telemetry: ");
        _instance->_serial.println(val > 0 ? "ON" : "OFF");
    }
}
