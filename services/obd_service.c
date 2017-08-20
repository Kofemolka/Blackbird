/*
 * obd_service.c
 *
 *  Created on: 16 ρεπο. 2017 π.
 *      Author: ayakuba
 */
#include "afx.h"

#include "ble_dis.h"
#include "services.h"

#define OPCODE_LENGTH 1
#define HANDLE_LENGTH 2

#define MANUFACTURER_NAME                "Kofe.shop"
/**@brief 128-bit UUID base List. */

#define MAX_DYNAMICS_LEN (NRF_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH)

#define configOBD_SERVICE_CHARS	2

typedef struct
{
	ble_gatts_char_handles_t	char_handle;
	bool						notify_enabled;
	uint8_t						value;
} obd_service_char_data_t;

typedef struct
{
	uint16_t service_handle;

	obd_service_char_data_t chars[configOBD_SERVICE_CHARS];
} obd_service_data_t;

static obd_service_data_t m_obd_service_data;
extern uint16_t                 m_conn_handle;

void on_odb_service_ble_evt(ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    NRF_LOG_INFO("on_odb_service_ble_evt: handle=%x len=%d data=%x\n\r", p_evt_write->handle, p_evt_write->len, p_evt_write->data);

    for(int c=0; c<configOBD_SERVICE_CHARS;c++)
    {
		if ( (p_evt_write->handle == m_obd_service_data.chars[c].char_handle.cccd_handle) &&
			 (p_evt_write->len == 2) )
		{
			bool notif_enabled;

			notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

			if (m_obd_service_data.chars[c].notify_enabled != notif_enabled)
			{
				m_obd_service_data.chars[c].notify_enabled = notif_enabled;
			}
		}
    }
}

static uint32_t add_char(const uint8_t uuid_type, uint8_t index, uint16_t uuid)
{
    ret_code_t          err_code;
   ble_gatts_char_md_t char_md;
   ble_gatts_attr_md_t cccd_md;
   ble_gatts_attr_t    attr_char_value;
   ble_uuid_t          char_uuid;
   ble_gatts_attr_md_t attr_md;

   memset(&cccd_md, 0, sizeof(cccd_md));

   cccd_md.vloc = BLE_GATTS_VLOC_STACK;
   BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
   BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

   memset(&char_md, 0, sizeof(char_md));

   char_md.char_props.read   = 1;
   char_md.char_props.notify = 1;
   char_md.p_char_user_desc  = NULL;
   char_md.p_char_pf         = NULL;
   char_md.p_user_desc_md    = NULL;
   char_md.p_cccd_md         = &cccd_md;
   char_md.p_sccd_md         = NULL;

   char_uuid.type = uuid_type;
   char_uuid.uuid = uuid;

   memset(&attr_md, 0, sizeof(attr_md));

   BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
   BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

   attr_md.vloc    = BLE_GATTS_VLOC_STACK;
   attr_md.rd_auth = 0;
   attr_md.wr_auth = 0;
   attr_md.vlen    = 0;

   memset(&attr_char_value, 0, sizeof(attr_char_value));

   attr_char_value.p_uuid    = &char_uuid;
   attr_char_value.p_attr_md = &attr_md;
   attr_char_value.init_len  = 1;
   attr_char_value.init_offs = 0;
   attr_char_value.max_len   = 1;
   m_obd_service_data.chars[index].value = 0xBE;
   attr_char_value.p_value   = &m_obd_service_data.chars[index].value;

   err_code = sd_ble_gatts_characteristic_add(m_obd_service_data.service_handle,
											  &char_md,
											  &attr_char_value,
											  &m_obd_service_data.chars[index].char_handle);
   APP_ERROR_CHECK(err_code);
}

uint32_t obd_service_register()
{
    ble_uuid_t service_uuid;
	ret_code_t err_code;

	service_uuid.uuid = BLE_UUID_OBD_SERVICE;

	err_code = sd_ble_uuid_vs_add(&m_base_uuid128, &service_uuid.type);
	APP_ERROR_CHECK(err_code);

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &m_obd_service_data.service_handle);
	APP_ERROR_CHECK(err_code);

	// Add characteristics
	APP_ERROR_CHECK(add_char(service_uuid.type, ID_CHAR_SPEED, BLE_UUID_CHAR_SPEED));
	APP_ERROR_CHECK(add_char(service_uuid.type, ID_CHAR_TEMP, BLE_UUID_CHAR_TEMP));
}


ret_code_t obd_service_init(void)
{
    ret_code_t      err_code;
    ble_dis_init_t  dis_init;

    err_code = obd_service_register();
    APP_ERROR_CHECK(err_code);

    // Initialize Device Information Service.
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)MANUFACTURER_NAME);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err_code = ble_dis_init(&dis_init);
    APP_ERROR_CHECK(err_code);
}

void ble_service_notify(uint8_t index, char value)
{
    uint32_t err_code = NRF_SUCCESS;

	if (value != m_obd_service_data.chars[index].value)
	{
		ble_gatts_value_t gatts_value;

		// Initialize value struct.
		memset(&gatts_value, 0, sizeof(gatts_value));

		gatts_value.len     = sizeof(uint8_t);
		gatts_value.offset  = 0;
		gatts_value.p_value = &value;

		// Update database.
		err_code = sd_ble_gatts_value_set(m_conn_handle,
										  m_obd_service_data.chars[index].char_handle.value_handle,
										  &gatts_value);
		if (err_code == NRF_SUCCESS)
		{
			// Save new battery value.
			m_obd_service_data.chars[index].value = value;
		}
		else
		{
			return;
		}

		if(m_conn_handle != BLE_CONN_HANDLE_INVALID && m_obd_service_data.chars[index].notify_enabled)
		{
			ble_gatts_hvx_params_t hvx_params;

			uint16_t hvx_len =1;

			// Initialize value struct.
			memset(&hvx_params, 0, sizeof(hvx_params));

			hvx_params.handle = m_obd_service_data.chars[index].char_handle.value_handle;
			hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
			hvx_params.p_len  = &hvx_len;
			hvx_params.offset = 0;
			hvx_params.p_data = &m_obd_service_data.chars[index].value;

			err_code = sd_ble_gatts_hvx(m_conn_handle, &hvx_params);
		}
		else
		{
			err_code = NRF_ERROR_INVALID_STATE;
		}
	}
}
