// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <string.h>

#include <drivers/iommu/iommu.h>
#include <drivers/mailbox/mailbox.h>
#include <drivers/otp/otp.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/log.h>
#include <libs/utils-def.h>

#include "ipc.h"
#include "protocol.h"

int risc0_ipc_otp_init(void)
{
	otp_t *otp_dump = otp_get_dump();
	if (!otp_dump)
		return -ENULL;

	memset(otp_dump->duk, 0, sizeof(otp_dump->duk));

	return 0;
}

void risc0_ipc_otp_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                           risc0_ipc_resp_param_t *resp_param)
{
	if ((link_id != FIFO4) && (link_id != FIFO5)) {
		ERROR("Power domain request is allowed from secure world only\n");
		return;
	}

	switch (cmd->hdr.func) {
	case RISC0_IPC_OTP_FUNC_GET_DUMP:
		otp_t *otp_dump = otp_get_dump();
		if (!otp_dump) {
			resp_param->otp.get_dump.error = -ENULL;
			return;
		}

		uint16_t size = cmd->param.otp.get_dump.size;
		uintptr_t end = (uintptr_t)otp_dump + (uintptr_t)size;

		// Check that we copy valid part of OTP dump to dest
		if (end > (uintptr_t)otp_dump + sizeof(otp_t)) {
			resp_param->otp.get_dump.error = -EINVALIDLENGTH;
			return;
		}

		uintptr_t buf;
		iommu_regs_t *iommu = iommu_get_registers();

		// Protect firmware from writing in it's own address space (first 4 GB)
		if (cmd->param.otp.get_dump.buf <= UINTPTR_MAX)
			panic_handler("The address[0x%llu] must be outside 32bit address space\n",
			              cmd->param.otp.get_dump.buf);

		buf = iommu_map(iommu, cmd->param.otp.get_dump.buf);
		if (!buf)
			panic_handler("No free memory\n");

		memcpy((void *)buf, otp_dump, size);
		wmem_barrier();

		iommu_unmap(iommu, buf);

		resp_param->otp.get_dump.error = 0;

		break;
	default:
		ERROR("Unsupported OTP service command=%d\n", cmd->hdr.func);
		break;
	}
}
