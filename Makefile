MODULES = dmem trie unit

help:
	@echo "Plib - Palm's C utility library"
	@echo "Available commands:"
	@echo "  help     - Displays this message."
	@echo "  tests    - Builds and executes all tests."
	@echo "  clean    - Cleans all modules from generated files."

tests:
	@echo "Building tests ..."
	$(foreach MODULE, $(MODULES),cd $(MODULE) && make tests$(\n))
	@echo "Running tests ..."
	@$(foreach MODULE, $(MODULES),./$(MODULE)/tests$(\n))

clean:
	$(foreach MODULE, $(MODULES),cd $(MODULE) && make clean$(\n))

define \n


endef
