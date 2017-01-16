#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
/* Create MFRC522 instance */
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
	/* Initialize serial communications with the PC */
	Serial.begin(9600);	
	/* Initialize SPI bus */
	SPI.begin();
	/* Initialize MFRC522 instance */
	mfrc522.PCD_Init();
}

void loop() {
	/* Look for new cards */
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	/* Select one of the cards */
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	/* Show UID on serial monitor */
	String content= "";
	for (byte i = 0; i < mfrc522.uid.size; i++) {
		content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(mfrc522.uid.uidByte[i], HEX));
	}
	Serial.println("@" + content);

	/* Authenticate sector 8 of RFID Tag with default key */
	MFRC522::MIFARE_Key key;

	/* Default password for sectors is 0xFF */
	for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++)
		key.keyByte[i] = 0xFF;

	byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 8, &key, &mfrc522.uid);
	if (status != MFRC522::STATUS_OK) {
		Serial.print("Not authenticated: ");
		Serial.println(GetStatusCodeName(status));
	}

	/* Get user command */
	Serial.println("Waiting ...");
	char inputChar = 0;
	String inputLine = "";

	do {
		if (Serial.available() > 0) {
			inputChar = Serial.read();
			inputLine.concat(inputChar);
		}
	} while (inputChar != ';');

	Serial.println("r" + inputLine);

	/* Read 32 bit value from sector 8 */
	if (inputLine[0] == 'R') {
		int32_t value;
		char *value_p = (char *)&value;

		mfrc522.MIFARE_GetValue(8, &value);

		for (byte i = 0; i < 4; i++) {
			Serial.print(char(value_p[i]));
		}
		Serial.println();

	/* Write 32 bit value to sector 8 */
	} else if (inputLine[0] == 'W') {
		int32_t value = 0;
		char *value_p = (char *)&value;

		for (byte i = 0; i < 4; i++) {
			value_p[i] = inputLine[i + 2];
		}

		for (byte i = 0; i < 4; i++) {
			Serial.print(char(value_p[i]));
		}
		Serial.println();

		mfrc522.MIFARE_SetValue(8, value);
	}

	return;
}
