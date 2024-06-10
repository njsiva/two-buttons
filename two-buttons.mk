######################################
#
# two-buttons
#
######################################

# where to find the source code - locally in this case
TWO_BUTTONS_SITE_METHOD = local
TWO_BUTTONS_SITE = $($(PKG)_PKGDIR)/

# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
TWO_BUTTONS_VERSION = 1

# dependencies (list of other buildroot packages, separated by space)
TWO_BUTTONS_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
TWO_BUTTONS_BUNDLES = two-buttons.lv2

# call make with the current arguments and path. "$(@D)" is the build directory.
TWO_BUTTONS_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)/source


# build command
define TWO_BUTTONS_BUILD_CMDS
	$(TWO_BUTTONS_TARGET_MAKE)
endef

# install command
define TWO_BUTTONS_INSTALL_TARGET_CMDS
	$(TWO_BUTTONS_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
endef


# import everything else from the buildroot generic package
$(eval $(generic-package))
