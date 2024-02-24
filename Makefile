.PHONY: all check_kernel_cmdline check_kernel_version run_tests

# Define expected kernel command line arguments
EXPECTED_KERNEL_CMDLINE_ARGS := nosmap nosmep mitigations=off

# Define the exact expected kernel name
EXPECTED_KERNEL_NAME := 5.14.0+

# Main target
all: check_environment run_adaptors

# First make sure the environment is setup correctly
check_environment: check_kernel_version check_kernel_cmdline

# Check kernel command line for certain arguments
check_kernel_cmdline:
	@echo "Checking kernel command line arguments"
	@$(foreach arg,$(EXPECTED_KERNEL_CMDLINE_ARGS),\
		if grep --quiet $(arg) /proc/cmdline; then \
			echo "[*] $(arg) found"; \
		else \
			echo "[!] Required argument $(arg) NOT found in kernel command line"; \
			exit 1; \
		fi;)

# Check for a specific kernel name
check_kernel_version:
	@echo "Checking kernel name"
	@KERNEL_NAME=`uname -r`; \
	if [ "$$KERNEL_NAME" = "$(EXPECTED_KERNEL_NAME)" ]; then \
		echo "[*] Running expected kernel: $$KERNEL_NAME"; \
	else \
		echo "[!] Unexpected kernel version. Expected $(EXPECTED_KERNEL_NAME), but found $$KERNEL_NAME."; \
		exit 1; \
	fi

# Make sure adaptors work
run_adaptors:
	@echo "--- Running adaptors ---"
	@if mitigate_all.sh &> /dev/null; then \
		echo "[*] Adaptors ran successfully"; \
	else \
		echo "[!] Failed to run adaptors on all cores"; \
		exit 1; \
	fi
