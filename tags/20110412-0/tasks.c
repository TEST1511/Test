#include "main.h"
#include "utils.h"
#include "display.h"
#include "settings.h"
#include "menu_shortcuts.h"
#include "firmware.h"

#include "tasks.h"

void set_intermediate_iso();

void start_up() {
	// Wait for camera to settle down
	SleepTask(1000);

	// Enable (hidden) CFn.8 for ISO H
	if (!cameraMode.CfExtendIso)
		SendToIntercom(EVENT_SET_CFN_EXTISO, 1, 1);

	// Enable realtime ISO change
	SendToIntercom(EVENT_SET_REALTIMEISO0, 0, 0);
	SendToIntercom(EVENT_SET_REALTIMEISO1, 0, 0);

	// Read (and apply) settings from file
	settings_read();
	settings_apply();
}

void dp_action() {
	if (settings.shortcuts_menu || cameraMode.AEMode > 6) {
		menu_shortcuts_start();
	} else {
		set_intermediate_iso();
		display_refresh();
	}
}

void set_metering_spot() {
	int metering_spot = METERING_MODE_SPOT;

	pressButton_(BUTTON_SET);
	SendToIntercom(EVENT_SET_METMODE, 1, metering_spot);
	eventproc_PrintICUInfo();

	beep();
}

void set_whitebalance_colortemp() {
	pressButton_(BUTTON_SET);
	SendToIntercom(EVENT_SET_WBMODE,    1, WB_MODE_COLORTEMP);
	SendToIntercom(EVENT_SET_COLORTEMP, 2, settings.color_temp);
	eventproc_PrintICUInfo();

	beep();
}

void set_iso_high() {
	pressButton_(BUTTON_SET);
	SendToIntercom(EVENT_SET_ISO, 2, 0x6F);
	eventproc_PrintICUInfo();

	beep();
}

void toggle_raw_jpeg() {
	if (cameraMode.AEMode > 6) {
		// Only for non-creative modes
		SendToIntercom(EVENT_SET_QUALITY, 1, cameraMode.QualityRaw ^ 0x03);
	}
}

void toggle_CfMLU() {
	SendToIntercom(EVENT_SET_CFN_MLU, 1, cameraMode.CfMLU ^ 0x01);
}

void toggle_CfFlashSyncRear() {
	SendToIntercom(EVENT_SET_CFN_FLASHSYNCR, 1, cameraMode.CfFlashSyncRear ^ 0x01);
}

void set_intermediate_iso() {
	if (cameraMode.AEMode < 6) {
		int iso = iso_roll(cameraMode.ISO);
		SendToIntercom(EVENT_SET_ISO, 2, iso);
	}

	SleepTask(50);
}

void restore_iso() {
	int iso;

	if (cameraMode.ISO >= 0x68) {
		iso = 0x68;
	} else if (cameraMode.ISO >= 0x60) {
		iso = 0x60;
	} else if (cameraMode.ISO >= 0x58) {
		iso = 0x58;
	} else if (cameraMode.ISO >= 0x50) {
		iso = 0x50;
	} else {
		iso = 0x48;
	}

	SendToIntercom(EVENT_SET_ISO, 2, iso);
}

void restore_wb() {
	if (cameraMode.WB == WB_MODE_COLORTEMP) {
		SendToIntercom(EVENT_SET_WBMODE, 1, WB_MODE_AUTO);
	}
}

void restore_metering() {
	if (cameraMode.MeteringMode == METERING_MODE_SPOT)
		SendToIntercom(EVENT_SET_METMODE, 1, METERING_MODE_EVAL);
}