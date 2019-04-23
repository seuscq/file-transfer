
static int __main_terminated;

static void print_prompt(void)
{
	printf("[gatt test]# ");
	fflush(stdout);
}

#define LOG_BUF_MAX_LEN	256
static char log_buf[LOG_BUF_MAX_LEN];
static void rtb_printf(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(log_buf, sizeof(log_buf), fmt, args);
	va_end(args);

	printf("%s", log_buf);
	print_prompt();
}

static int parse_arguments(char *arguments, char **argv)
{
	int argc;
	char *p;
	char *parg = (char *)arguments;

	if (!arguments || !argv)
		return 0;

	argc = 0;
	while ((p = strsep(&parg, " \t")))
		if (p[0])
			argv[argc++] = p;

	return argc;
}
static void cmd_quit(const char *arg)
{
	pr_info("Quit");
	__main_terminated = 1;
}

static void cmd_test(const char *arg)
{
	print_prompt();
}

static void cmd_init(const char *arg)
{
	pr_info("Init");
	CSM_init();
}

static void cmd_deinit(const char *arg)
{
	pr_info("DeInit");
	CSM_deinitGatts();
}


static void cmd_add_service(const char *arg)
{
	char *uuid;

	pr_info("add service");

	uuid = WIFI_SERVICE_UUID; 
	CSM_addService(1, uuid, 1, 8);
}

static void cmd_add_char(const char *arg)
{
	char *uuid;
	uint32_t prop, perm;
	
	pr_info("add char");
	//uuid = WIFI_SSID_CHAR_UUID; 
	prop = 0x0a;
	perm = 0x03;

	CSM_addChar(1,(uint32_t)service_handle, WIFI_SSID_CHAR_UUID, prop, perm);
	CSM_addChar(1,(uint32_t)service_handle, WIFI_STATUS_CHAR_UUID, prop, perm);
}

static void cmd_add_desc(const char *arg)
{
	char *uuid;
	uint32_t perm;
	
	pr_info("add desc");
	uuid = WIFI_STATUS_DESC_LONG_UUID; 
	perm = 0x03;

	CSM_addDesc(1,(uint32_t)service_handle, uuid, perm);
}

static void cmd_start_service(const char *arg)
{
	CSM_startService(1,(uint32_t)service_handle, 0);
}

static void cmd_stop_service(const char *arg)
{
	CSM_stopService(1,(uint32_t)service_handle);
}

static void cmd_setcb(const char *arg)
{
	pr_info("setcb");
	CSM_setCallback(&sample_cbs);
	// for quick test
#define TEST
#ifdef TEST
	cmd_init(NULL);
	cmd_add_service(NULL);
	cmd_add_char(NULL);
	cmd_add_desc(NULL);
	cmd_start_service(NULL);
	enableAdv(1);
	setAdvData();
#endif
	
}
/*INT32 CSM_sendResponse(INT32 conn_id,INT32 trans_id,INT32 status,
						INT32 handle, CHAR * p_value,INT32 value_len,
						INT32 auth_req) */
/*
 * TODO: status: rsp code?
 * 	   : auth_req: error rsp error code?
 * 	   Don't use this command.
 */
static void cmd_rsp(const char *arg)
{
	uint32_t handle = 0x0003; // this is the handle of test handle
	char p_value[2] = "ab";
	uint32_t value_len = 2;
	uint32_t status = 0x0b; // status could be 0x0b(read rsp) or 0x0d(read blob rsp)
	uint32_t auth_req = 0x00;
	CSM_sendResponse(0, 0, status, handle, p_value, value_len, auth_req);
}

/*
 * INT32 CSM_sendIndication(INT32 server_if, INT32 handle, 
 * INT32 conn_id, INT32 fg_confirm, CHAR * p_value, INT32 value_len)
 */
static void cmd_notify(const char *arg)
{
	uint32_t handle = 0x0005;
	uint32_t notify = true;

	char *p_value = "abc";
	uint32_t value_len = 3;
	CSM_sendIndication(0, handle, 0, notify, p_value, value_len);
}

static void cmd_unreg_server(const char *arg)
{
	CSM_unregisterService(1);
}

static void cmd_spawn(const char *arg)
{
	// spawn a second thread
	spawn_thread();
}

static const struct {
	const char *cmd;
	const char *arg;
	void (*func) (const char *arg);
	const char *desc;
	char * (*gen) (const char *text, int state);
	void (*disp) (char **matches, int num_matches, int max_length);
} cmd_table[] = {
	{ "quit",         NULL,       cmd_quit, "Quit program" },
	{ "exit",         NULL,       cmd_quit },
	{ "test",         NULL,       cmd_test },
	{ "init",         NULL,       cmd_init },
	{ "as",   		  NULL,       cmd_add_service },
	{ "ac",   		  NULL,       cmd_add_char},
	{ "ad",   		  NULL,       cmd_add_desc},
	{ "start",   	  NULL,       cmd_start_service},
	{ "stop",   	  NULL,       cmd_stop_service},
	{ "rsp", 		  NULL,		  cmd_rsp},
	{ "notify", 	  NULL,		  cmd_notify},
	{ "unreg",   	  NULL,       cmd_unreg_server},
	{ "setcb",   	  NULL,       cmd_setcb},
	{ "deinit",       NULL,       cmd_deinit },
	{ "spawn",       NULL,        cmd_spawn},
	{ "help" },
	{ }
};

static void process_cmdline(char *input_str, uint32_t len)
{
	char *cmd, *arg;
	int i;

	/* If user enter CTL + d, program will read an EOF and len
	 * is zero */
	if (!len) {
		pr_info("empty command\n");
		goto done;
	}

	if (!strlen(input_str))
		goto done;

	if (input_str[0] == '\n' || input_str[0] == '\r')
		goto done;

	if (input_str[len - 1] == '\n' || input_str[len - 1] == '\r')
		input_str[len - 1] = 0;

	/* rtb_printf("input_str: %s\n", input_str); */

	cmd = strtok_r(input_str, " ", &arg);
	if (!cmd)
		goto done;

	if (arg) {
		int len = strlen(arg);
		if (len > 0 && arg[len - 1] == ' ')
			arg[len - 1] = '\0';
	}
	/* rtb_printf("cmd %s -\n", cmd); */
	/* rtb_printf("arg %s -\n", arg); */

	for (i = 0; cmd_table[i].cmd; i++) {
		if (strcmp(cmd, cmd_table[i].cmd))
			continue;

		if (cmd_table[i].func) {
			cmd_table[i].func(arg);
			goto done;
		}
	}

	if (strcmp(cmd, "help")) {
		rtb_printf("Invalid command\n");
		goto done;
	}

	rtb_printf("Available commands:\n");

	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			rtb_printf("  %s %-*s %s\n", cmd_table[i].cmd,
					(int)(25 - strlen(cmd_table[i].cmd)),
					cmd_table[i].arg ? : "",
					cmd_table[i].desc ? : "");
	}
done:
	return;
}

static void stdin_read_handler(int fd)
{
	ssize_t read;
	size_t len = 0;
	char *line = NULL;

	read = getline(&line, &len, stdin);
	if (read < 0)
		return;

	if (read <= 1) {
		print_prompt();
		return;
	}

	line[read-1] = '\0';

	process_cmdline(line, strlen(line) + 1);
}

static int sigfd_setup(void)
{
	sigset_t mask;
	int fd;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		perror("Failed to set signal mask");
		return -1;
	}

	fd = signalfd(-1, &mask, 0);
	if (fd < 0) {
		perror("Failed to create signal descriptor");
		return -1;
	}

	return fd;
}
int main()
{
	int result;
	int sigfd;
	struct pollfd pfd[2];

	sigfd = sigfd_setup();
	if (sigfd < 0)
		return EXIT_FAILURE;

	pfd[0].fd = sigfd;
	pfd[0].events = POLLIN | POLLHUP | POLLERR;
	pfd[1].fd = fileno(stdin);
	pfd[1].events = POLLIN | POLLHUP | POLLERR;

	print_prompt();

	while (!__main_terminated) {
		pfd[0].revents = 0;
		pfd[1].revents = 0;
		if (poll(pfd, 2, -1) == -1) {
			if (errno == EINTR)
				continue;
			pr_error("Poll error: %s", strerror(errno));
			return EXIT_FAILURE;
		}

		if (pfd[0].revents & (POLLHUP | POLLERR)) {
			pr_error("Poll rdhup or hup or err");
			return EXIT_FAILURE;
		}

		if (pfd[1].revents & (POLLHUP | POLLERR)) {
			pr_error("Poll rdhup or hup or err");
			return EXIT_FAILURE;
		}

		if (pfd[0].revents & POLLIN) {
			struct signalfd_siginfo si;
			ssize_t ret;

			ret = read(pfd[0].fd, &si, sizeof(si));
			if (ret != sizeof(si))
				return EXIT_FAILURE;
			switch (si.ssi_signo) {
			case SIGINT:
			case SIGTERM:
				break;
			}
		}

		if (pfd[1].revents & POLLIN)
			stdin_read_handler(pfd[1].fd);
	}

	pr_info("exit");

	return 0;
}
