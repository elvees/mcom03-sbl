// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __SBIMAGE_H__
#define __SBIMAGE_H__

typedef struct {
	unsigned int objType : 3; //!< Тип данных, которые содержит образ (Смотри в таблице 2.4)
	unsigned int
		checksum : 1; //!< 0: Проверка целостности не проводится 1: Проверка целостности проводится
	unsigned int encrypted : 1; //!< 0: Данные не зашифрованы 1: Данные зашифрованы
	unsigned int
		signOfEncrypted : 1; //!< 0: Поле Payload digest содержит хэш нешифрованных данных 1: Поле Payload digest содержит хэш шифрованных данных
	unsigned int signedObj : 1; //!< 0: Объект не подписан 1: Объект подписан
	unsigned int
		skipHeaderHash : 1; //!< 1: В режиме bs_en=0 пропустить проверку контрольной суммы заголовка
	unsigned int Reserved : 24; //!< Зарезервировано
} flags_t;

typedef struct {
	unsigned int hId; //!< ID типа образа, всегда 0x53424d47 ("SBMG")
	unsigned int plSize; //!< Размер данных в байтах, следующих сразу за этим заголовком
	unsigned int lAddr; //!< Адрес, по которому загружаются данные
	unsigned int eAddr; //!< Адрес входа в программу (если образ содержит исполняемую программу)

	union {
		unsigned int flags;
		flags_t flagsBits; //!< Поле Flags
	};
	unsigned int aesKeyNum; //!< Номер ключа для расшифровки (используются младшие 16 бит)
	unsigned int certId; //!< ID номер, если это сертификат
	unsigned int
		signCertId; //!< ID номер сертификата, который использовался для подписи данных (если использовался)
	unsigned char
		plDgst[32]; //!< Хэш данных, следующих за этим заголовком (Функция хэширования SHA-256)
	unsigned char hDgst
		[32]; //!< Хэш заголовка (Функция хэширования SHA-256). При его расчёте это поле принимается равным 0
} sbimghdr_t;

#endif /* __SBIMAGE_H__ */
