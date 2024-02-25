.PHONY: all check_kernel_cmdline check_kernel_version run_adaptors run_unit_tests

# ANSI color codes
RED := "\033[1;31m"
GREEN := "\033[1;32m"
YELLOW := "\033[1;33m"
BOLD := "\033[1m"
NO_COLOR := "\033[0m"

# Define expected kernel command line arguments
EXPECTED_KERNEL_CMDLINE_ARGS := nosmap nosmep mitigations=off

# Define the exact expected kernel name
EXPECTED_KERNEL_NAME := 5.14.0+

# Main target
all: check_environment run_adaptors run_unit_tests
	@printf "\n"

# First make sure the environment is setup correctly
check_environment: check_kernel_version check_kernel_cmdline
	@printf "\n[ %bOK%b ] Environment Validated\n" $(GREEN) $(NO_COLOR)

# Check kernel command line for certain arguments
check_kernel_cmdline:
	@printf "\n%b*------ Verifying kernel command line arguments ------*%b\n" $(BOLD) $(NO_COLOR)
	@$(foreach arg,$(EXPECTED_KERNEL_CMDLINE_ARGS),\
		if grep --quiet $(arg) /proc/cmdline; then \
			printf "[%b*%b] %b$(arg)%b found\n" $(GREEN) $(NO_COLOR) $(GREEN) $(NO_COLOR); \
		else \
			printf "%b[!]%b Required argument %b$(arg)%b NOT found in kernel command line%b\n" $(RED) $(NO_COLOR) $(RED) $(NO_COLOR); \
			exit 1; \
		fi;)

# Check for a specific kernel name
check_kernel_version:
	@printf "\n%b*------ Verifying kernel version ------*%b\n" $(BOLD) $(NO_COLOR)
	@KERNEL_NAME=`uname -r`; \
	if [ "$$KERNEL_NAME" = "$(EXPECTED_KERNEL_NAME)" ]; then \
		printf "[%b*%b] Running expected kernel: %b$$KERNEL_NAME%b\n" $(GREEN) $(NO_COLOR) $(GREEN) $(NO_COLOR); \
	else \
		printf "%b[!]%b Unexpected kernel version. Expected %b$(EXPECTED_KERNEL_NAME)%b, but found %b$$KERNEL_NAME%b\n" $(RED) $(NO_COLOR) $(YELLOW) $(NO_COLOR) $(RED) $(NO_COLOR); \
		exit 1; \
	fi

# Make sure adaptors work
run_adaptors:
	@printf "\n%b*------ Testing Adaptors ------*%b\n" $(BOLD) $(NO_COLOR)
	@if mitigate_all.sh &> /dev/null; then \
		printf "[%b*%b] Adaptors ran successfully\n" $(GREEN) $(NO_COLOR); \
	else \
		printf "%b[!]%b Failed to run adaptors on all cores\n" $(RED) $(NO_COLOR); \
		exit 1; \
	fi

# Run unit tests
run_unit_tests:
	@printf "\n%b*------ Running Unit Tests ------*%b\n" $(BOLD) $(NO_COLOR)
	$(MAKE) -C unit-tests clean &> /dev/null
	@if $(MAKE) -C unit-tests run_tests; then \
		printf "\n[ %bOK%b ] All unit tests passed successfully!\n" $(GREEN) $(NO_COLOR); \
	else \
		printf "%b[!]%b Failed to pass all unit tests\n" $(RED) $(NO_COLOR); \
		exit 1; \
	fi

