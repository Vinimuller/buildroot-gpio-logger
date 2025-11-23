################################################################################
#
# gpio-logger
#
################################################################################

GPIO_LOGGER_VERSION = 1.0
GPIO_LOGGER_SITE = package/gpio-logger/src
GPIO_LOGGER_SITE_METHOD = local
TARGET_CFLAGS = -Wall -O2
TARGET_LDFLAGS = -lgpiod

define GPIO_LOGGER_BUILD_CMDS
	$(echo "ESTOU BUILDANDOee")
	$(MAKE) CC="$(TARGET_CC)" \
	        CFLAGS="$(TARGET_CFLAGS)" \
	        LDFLAGS="$(TARGET_LDFLAGS)" -C $(@D)
endef

define GPIO_LOGGER_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/gpio-logger $(TARGET_DIR)/usr/bin/gpio-logger
	$(INSTALL) -D $(GPIO_LOGGER_PKGDIR)/S98gpio-logger \
		$(TARGET_DIR)/etc/init.d/S98gpio-logger
	$(INSTALL) -D $(GPIO_LOGGER_PKGDIR)/files/gpio-logger.conf \
		$(TARGET_DIR)/etc/gpio-logger.conf
endef

$(eval $(generic-package))

