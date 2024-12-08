
# -- V A R I A B L E S --------------------------------------------------------

# build script
override script := $(CURDIR)/make.sh



# -- S P E C I A L  T A R G E T S ---------------------------------------------

.PHONY: all rm clean fclean re test


# -- T A R G E T S ------------------------------------------------------------

all:
	@$(script)

rm:
	@$(script) rm

clean:
	@$(script) clean

fclean:
	@$(script) fclean

re:
	@$(script) re

test:
	@$(script) 'test'
