include $(TOPDIR)/rules.mk

PKG_NAME:=elcorecllib
PKG_RELEASE:=1
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/elcorecllib
	SECTION:=libs
	CATEGORY:=Development
	DEPENDS:=@TARGET_elvees_mcom03 +kmod-elvees-elcore50 +boost-filesystem +boost-regex
	TITLE:=Development files for the elcorecl library
endef

TARGET_CFLAGS += -I$(STAGING_DIR_ROOT)/usr/include

define Package/elcorecllib/install
	$(INSTALL_DIR) $(1)/usr/include/elcorecl
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/elcorecl/* $(1)/usr/include/elcorecl
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libelcore.so  $(1)/usr/lib/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libelcorecl.so  $(1)/usr/lib/
endef

$(eval $(call BuildPackage,elcorecllib))

