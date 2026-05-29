/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "format_common.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "media_hal_common.h"
#include "securec.h"

#define MODULE_NAME "FormatCommon"

static const CodecFormat g_codecTypeToCodecFormat[CODEC_TYPE_BUT + 1] = {
    CODEC_H264, CODEC_H265, CODEC_JPEG, CODEC_AAC, CODEC_G711A, CODEC_G711U, CODEC_PCM, CODEC_MP3, CODEC_G726,
    CODEC_BUT
};

static const CodecType g_codecFormatToCodecType[CODEC_BUT + 1] = {
    CODEC_TYPE_H264, CODEC_TYPE_H265, CODEC_TYPE_JPEG, CODEC_TYPE_AAC, CODEC_TYPE_G711A, CODEC_TYPE_G711U,
    CODEC_TYPE_PCM, CODEC_TYPE_MP3, CODEC_TYPE_G726, CODEC_TYPE_BUT
};

static const SubtitleFormat g_subTypeToSubFormat[FORMAT_SUB_TYPE_BUTT + 1] = {
    FORMAT_SUB_ASS, FORMAT_SUB_LRC, FORMAT_SUB_SRT, FORMAT_SUB_SMI, FORMAT_SUB_SUB, FORMAT_SUB_TXT,
    FORMAT_SUB_HDMV_PGS, FORMAT_SUB_DVB_SUB, FORMAT_SUB_DVD_SUB, FORMAT_SUB_TTML, FORMAT_SUB_WEBVTT,
    FORMAT_SUB_BUTT
};

static const PacketType g_frameTypeToPacketType[FRAME_TYPE_BUT + 1] = {
    PACKET_TYPE_NONE, PACKET_TYPE_AUDIO, PACKET_TYPE_VIDEO, PACKET_TYPE_IMAGE, PACKET_TYPE_SUB, PACKET_TYPE_DATA,
    PACKET_TYPE_BUT
};

static const FrameType g_packetTypeToFrameType[PACKET_TYPE_BUT + 1] = {
    FRAME_TYPE_NONE, FRAME_TYPE_AUDIO, FRAME_TYPE_VIDEO, FRAME_TYPE_IMAGE, FRAME_TYPE_SUB, FRAME_TYPE_DATA,
    FRAME_TYPE_BUT
};

CodecFormat CodecTypeToCodecFormat(CodecType type)
{
    return g_codecTypeToCodecFormat[type];
}

CodecType CodecFormatToCodecType(CodecFormat type)
{
    return g_codecFormatToCodecType[type];
}

SubtitleFormat SubTypeToSubFormat(SubtitleFormatType type)
{
    return g_subTypeToSubFormat[type];
}

void FormatPacketToFrame(const FormatPacket *packet, FormatFrame *frame)
{
    if (frame == NULL || packet == NULL || packet->packetType > PACKET_TYPE_BUT) {
        return;
    }

    frame->frameType = g_packetTypeToFrameType[packet->packetType];
    frame->trackId = packet->trackId;
    frame->isKeyFrame = packet->isKeyFrame;
    frame->timestampUs = packet->timestampUs;
    frame->durationUs = packet->durationUs;
    frame->data = packet->data;
    frame->len = packet->len;
    frame->frameIndex = packet->frameIndex;
    frame->position = packet->position;
    frame->itemCnt = packet->itemCnt;
    frame->item = (ParameterItem *)packet->item;
}

void FormatFrameToPacket(const FormatFrame *frame, FormatPacket *packet)
{
    if (frame == NULL || packet == NULL || frame->frameType > FRAME_TYPE_BUT) {
        return;
    }

    packet->packetType = g_frameTypeToPacketType[frame->frameType];
    packet->trackId = frame->trackId;
    packet->isKeyFrame = frame->isKeyFrame;
    packet->timestampUs = frame->timestampUs;
    packet->durationUs = frame->durationUs;
    packet->data = frame->data;
    packet->len = frame->len;
    packet->frameIndex = frame->frameIndex;
    packet->position = frame->position;
    packet->itemCnt = frame->itemCnt;
    packet->item = (OptionalItem *)frame->item;
}

