
error:
	@echo 'ERROR: The purpose of this makefile is to create a type-word list to stdout'
	@echo 'try make twlist'

twlist:
	@${NICETEXT_BIN_DIR}txt2dct name_family name_female name_male name_other place
	@${NICETEXT_BIN_DIR}txt2dct name_family | awk -f posplr.awk
	@${NICETEXT_BIN_DIR}txt2dct name_family name_female name_male name_other | awk -f pos.awk
