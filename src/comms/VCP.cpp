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
    if (_telemetryEnabled > 0 && _mc) {
        if (millis() - _lastTelemetryTime > 100) {
            BLDCMotor& motor = _mc->getMotor();
            if (_telemetryEnabled == 1) {
                _serial.print("V:"); _serial.print(motor.shaft_velocity, 2);
                _serial.print(" A:"); _serial.print(motor.shaft_angle, 3);
                _serial.print(" T:"); _serial.print(motor.target, 2);
                _serial.print(" Iq:"); _serial.print(motor.current.q, 3);
                _serial.print(" Id:"); _serial.println(motor.current.d, 3);
            } else if (_telemetryEnabled == 2) {
                PhaseCurrent_s currents = motor.current_sense->getPhaseCurrents();
                _serial.print("Ia:"); _serial.print(currents.a, 3);
                _serial.print(" Ib:"); _serial.print(currents.b, 3);
                _serial.print(" Ic:"); _serial.println(currents.c, 3);
            } else if (_telemetryEnabled == 3) {
                _serial.print("ADC_A:"); _serial.print(analogRead(PA0));
                _serial.print(" ADC_B:"); _serial.print(analogRead(PA1));
                _serial.print(" ADC_C:"); _serial.println(analogRead(PA2));
            }
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
        _instance->setTelemetry(val);
        _instance->_serial.print("Telemetry: ");
        _instance->_serial.println(val > 0 ? "ON" : "OFF");
    }
}
