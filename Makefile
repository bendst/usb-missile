all: 
	$(MAKE) -C driver all 
	$(MAKE) -C frontend all

clean:
	$(MAKE) -C driver clean
	$(MAKE) -C frontend clean
