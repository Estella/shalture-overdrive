SUBDIRS=$(LIBMOWGLI) include libathemecore modules src $(PODIR)
CLEANDIRS = ${SUBDIRS}
DISTCLEAN = extra.mk buildsys.mk config.log config.status atheme-services.pc

-include extra.mk
-include buildsys.mk

# explicit dependencies need to be expressed to ensure parallel builds don't die
libathemecore: include $(LIBMOWGLI)
modules: libathemecore
src: libathemecore

install-extra:
	@echo "----------------------------------------------------------------"
	@echo ">>> Remember to cd to ${prefix} and edit your config file.";
	@echo "----------------------------------------------------------------"
	i="shalture-services.pc"; \
	${INSTALL_STATUS}; \
	if ${MKDIR_P} ${DESTDIR}${libdir}/pkgconfig && ${INSTALL} -m 644 $$i ${DESTDIR}${libdir}/pkgconfig/$$i; then \
		${INSTALL_OK}; \
	else \
		${INSTALL_FAILED}; \
	fi

uninstall-extra:
	i="shalture-services.pc"; \
	if [ -f ${DESTDIR}${libdir}/pkgconfig/$$i ]; then \
		if rm -f ${DESTDIR}${libdir}/pkgconfig/$$i; then \
			${DELETE_OK}; \
		else \
			${DELETE_FAILED}; \
		fi \
	fi

buildsys.mk:
	@echo "Please run ./configure first."
	@exit 1
