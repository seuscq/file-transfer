
INT32 CSM_init()
{
	return gatt_thread_create();
}

INT32 CSM_addService(INT32 server_if, CHAR * service_uuid, 
				UINT8 is_primary, INT32 number)
{
	void *argv[4];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(server_if));
	argv[1] = service_uuid;
	argv[2] = (void *)(UINT_TO_PTR(is_primary));
	argv[3] = (void *)(UINT_TO_PTR(number));
	result = bta_submit_command_wait(GATTCMD_OP_ADD_SERVICE, 4, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_ADD_SERVICE, result);
		return (int)false;
	}

	return (int)true;
}

INT32 CSM_addChar(INT32 server_if, INT32 service_handle, 
				CHAR * uuid, INT32 properties, INT32 permissions)
{
	
	void *argv[5];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(server_if));
	argv[1] = (void *)(UINT_TO_PTR(service_handle));
	argv[2] = uuid;
	argv[3] = (void *)(UINT_TO_PTR(properties));
	argv[4] = (void *)(UINT_TO_PTR(permissions));
	result = bta_submit_command_wait(GATTCMD_OP_ADD_CHAR, 5, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_ADD_CHAR, result);
		return (int)false;
	}

	return (int)true;
}

INT32 CSM_addDesc(INT32 server_if, INT32 service_handle, 
				CHAR * uuid, INT32 permissions)
{
	void *argv[4];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(server_if));
	argv[1] = (void *)(UINT_TO_PTR(service_handle));
	argv[2] = uuid;
	argv[3] = (void *)(UINT_TO_PTR(permissions));
	result = bta_submit_command_wait(GATTCMD_OP_ADD_DESC, 4, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_ADD_DESC, result);
		return (int)false;
	}

	return (int)true;
}

INT32 CSM_startService(INT32 server_if, INT32 service_handle, INT32 transport)
{
	void *argv[3];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(server_if));
	argv[1] = (void *)(UINT_TO_PTR(service_handle));
	argv[2] = (void *)(UINT_TO_PTR(transport));
	result = bta_submit_command_wait(GATTCMD_OP_START_SERVICE, 3, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_START_SERVICE, result);
		return (int)false;
	}

	return (int)true;

}

INT32 CSM_stopService(INT32 server_if, INT32 service_handle)
{
	void *argv[2];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(server_if));
	argv[1] = (void *)(UINT_TO_PTR(service_handle));
	result = bta_submit_command_wait(GATTCMD_OP_STOP_SERVICE, 2, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_STOP_SERVICE, result);
		return (int)false;
	}

	return (int)true;

}

INT32 CSM_sendResponse(INT32 conn_id,INT32 trans_id,INT32 status,
						INT32 handle, CHAR * p_value,INT32 value_len,
						INT32 auth_req)
{
	void *argv[7];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(conn_id));
	argv[1] = (void *)(UINT_TO_PTR(trans_id));
	argv[2] = (void *)(UINT_TO_PTR(status));
	argv[3] = (void *)(UINT_TO_PTR(handle));
	argv[4] = (void *)(p_value);
	argv[5] = (void *)(UINT_TO_PTR(value_len));
	argv[6] = (void *)(UINT_TO_PTR(auth_req));
	result = bta_submit_command_wait(GATTCMD_OP_SEND_RSP, 7, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_SEND_RSP, result);
		return (int)false;
	}

	return (int)true;
}

INT32 CSM_sendIndication(INT32 server_if, INT32 handle, INT32 conn_id, 
						INT32 fg_confirm, CHAR * p_value, 
						INT32 value_len)
{
	void *argv[6];
	int result;

	argv[0] =(void *)(UINT_TO_PTR(server_if));
	argv[1] = (void *)(UINT_TO_PTR(handle));
	argv[2] = (void *)(UINT_TO_PTR(conn_id));
	argv[3] = (void *)(UINT_TO_PTR(fg_confirm));
	argv[4] = (void *)(p_value);
	argv[5] = (void *)(UINT_TO_PTR(value_len));
	result = bta_submit_command_wait(GATTCMD_OP_SEND_IND, 6, (void **)argv);
	if (result < 0) {
		pr_error("Run %04x command failed, %d",
			 GATTCMD_OP_SEND_RSP, result);
		return (int)false;
	}

	return (int)true;
}

INT32 CSM_unregisterService(INT32 server_if)
{	
	gatt_thread_quit();
	return 0;
}

INT32 CSM_deinitGatts()
{
	gatt_thread_quit();
	return 0;
}
