
# -- V A R I A B L E S --------------------------------------------------------

# build script
override script := $(CURDIR)/make.sh


# -- S P E C I A L  T A R G E T S ---------------------------------------------

.PHONY: all clean fclean


# -- T A R G E T S ------------------------------------------------------------

all:
	@$(script)

clean:
	@$(script) clean

fclean:
	@$(script) fclean
