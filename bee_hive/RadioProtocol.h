/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RADIOPROTOCOL_H_
#define RADIOPROTOCOL_H_

#include "stdint.h"
#include "easylink/EasyLink.h"

#define RADIO_CONCENTRATOR_ADDRESS     0x14
#define RADIO_EASYLINK_MODULATION      EasyLink_Phy_50kbps2gfsk

enum PacketType
{
    ACK_PACKET    = 0,
    SENSOR_PACKET = 1,
} PacketType;

#pragma pack(push,1)
struct PacketHeader {
    uint8_t sourceAddress;
    uint8_t packetType;
};

struct Data {
    uint16_t status;
    uint16_t current;
    uint16_t power;
    uint16_t voltage;
    uint16_t light;
    uint16_t temp1;
    uint16_t humi1;
    uint16_t temp2;
    uint16_t humi2;
    uint16_t pressure;
    uint16_t acc_voltage;
};

struct NodeData {
    uint16_t address;
    struct Data data;
    uint16_t rssi;
};

static uint8_t NodeDataSize = 13;

enum Registers {
    ADDRESS,
    STATUS,
    CURRENT,
    POWER,
    VOLTAGE,
    LIGHT,
    TEMP_1,
    HUMI_1,
    TEMP_2,
    HUMI_2,
    PRESSURE,
    ACC_VOLTAGE,
    RSSI,
};

struct SensorPacket {
    struct PacketHeader header;
    struct Data data;
    uint32_t time100MiliSec;
} SensorPacket;

struct AckPacket {
    struct PacketHeader header;
};
#pragma pack(pop)

#endif /* RADIOPROTOCOL_H_ */
