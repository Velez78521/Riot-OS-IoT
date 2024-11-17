# Nombre de tu aplicacin
APPLICATION = temperature-humidity


# Si no has configurado BOARD, esp8266-esp-12x será el predeterminado.
BOARD ?= esp32-wroom-32

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1


USEMODULE += shell
#USEMODULE += shell_commands
USEMODULE += xtimer

# Modules to include:
USEMODULE += dht
USEMODULE += fmt


FEATURES_OPTIONAL += periph_rtc


# Esto incluye el Makefile base de RIOT.
include $(RIOTBASE)/Makefile.include

