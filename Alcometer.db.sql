BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "user" (
	"id_user"	INTEGER NOT NULL UNIQUE,
	"login"	TEXT NOT NULL UNIQUE,
	"password"	TEXT NOT NULL,
	"fullname"	TEXT NOT NULL,
	PRIMARY KEY("id_user" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "health" (
	"давление"	REAL NOT NULL,
	"id_health"	INTEGER NOT NULL UNIQUE,
	"id_user"	INTEGER NOT NULL,
	"permill"	REAL NOT NULL,
	"temperature"	REAL NOT NULL,
	UNIQUE("id_health"),
	FOREIGN KEY("id_user") REFERENCES "user"("id_user"),
	PRIMARY KEY("id_health" AUTOINCREMENT)
);
INSERT INTO "user" VALUES (123,'BluetoothSerial.h. В этом случае следует обращаться к документации на используемый микроконтроллер и модуль Bluetooth, чтобы узнать, поддерживают ли они такие настройки и как их применить.','BluetoothSerial.h. В этом случае следует обращаться к документации на используемый микроконтроллер и модуль Bluetooth, чтобы узнать, поддерживают ли они такие настройки и как их применить.','BluetoothSerial.h. В этом случае следует обращаться к документации на используемый микроконтроллер и модуль Bluetooth, чтобы узнать, поддерживают ли они такие настройки и как их применить.');
INSERT INTO "user" VALUES (124,'1','1','1');
COMMIT;
