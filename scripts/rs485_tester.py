import serial
import time

# --- Konfigurazioa ---
PORT = 'COM4'      # Erabiltzailearen RS485 moldagailua
BAUD = 9600        # Gripperraren firmware konfiguratua
TIMEOUT = 1.0

def test_rs485():
    try:
        with serial.Serial(PORT, BAUD, timeout=TIMEOUT) as ser:
            print(f"--- RS485 Tester ({PORT} @ {BAUD} Baud) ---")
            
            # 1. PING Test
            payload = "1:PING:0\n"
            print(f"Tx: {payload.strip()}")
            ser.write(payload.encode('ascii'))
            line = ser.readline()
            print(f"Rx: {line}")
            time.sleep(1)
            
            # 2. Angle Request Test
            payload = "1:?A:0\n"
            print(f"Tx: {payload.strip()}")
            ser.write(payload.encode('ascii'))
            line = ser.readline()
            print(f"Rx: {line}")
            time.sleep(1)

            # 3. Enable Test
            payload = "1:EN:1\n"
            print(f"Tx: {payload.strip()}")
            ser.write(payload.encode('ascii'))
            line = ser.readline()
            print(f"Rx: {line}")

    except Exception as e:
        print(f"Errorea: {e}")

if __name__ == "__main__":
    test_rs485()
