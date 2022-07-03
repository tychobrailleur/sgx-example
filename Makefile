# To generate a proper enclave, it is recommended to follow below guideline to link the trusted libraries:
#    1. Link sgx_trts with the `--whole-archive' and `--no-whole-archive' options,
#       so that the whole content of trts is included in the enclave.
#    2. For other libraries, you just need to pull the required symbols.
#       Use `--start-group' and `--end-group' to link these libraries.
# Do NOT move the libraries linked with `--start-group' and `--end-group' within `--whole-archive' and `--no-whole-archive' options.
# Otherwise, you may get some undesirable errors.
Enclave_Link_Flags := -m64 -O0 -g -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles \
	-L/opt/intel/sgxsdk/lib64 \
	-Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive \
	-Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tcrypto -lsgx_tservice -Wl,--end-group \
	-Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
	-Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
	-Wl,--defsym,__ImageBase=0 -Wl,--gc-sections   \
	-Wl,--version-script=Enclave/Enclave.lds


App_Link_Flags := -m64 -O0 -g -L/opt/intel/sgxsdk/lib64 \
	-lsgx_urts -lpthread -lsgx_uae_service

.PHONT: all

all: app enclave.signed.so

# App Code
App/Enclave_u.c: $(SGX_EDGER8R) Enclave/Enclave.edl
	cd App && /opt/intel/sgxsdk/bin/x64/sgx_edger8r \
		--untrusted ../Enclave/Enclave.edl \
		--search-path ../Enclave \
		--search-path /opt/intel/sgxsdk/include

App/Enclave_u.o: App/Enclave_u.c
	gcc -m64 -O0  -fPIC -Wno-attributes -I/opt/intel/sgxsdk/include -c $< -o $@


App/app.o: App/app.c
	gcc -m64 -O0  -fPIC -Wno-attributes -I/opt/intel/sgxsdk/include  -c $< -o $@

app: App/Enclave_u.o App/app.o
	gcc  $^ -o $@ $(App_Link_Flags)



# Enclave Code
Enclave/Enclave_t.c: Enclave/Enclave.edl
	cd Enclave && /opt/intel/sgxsdk/bin/x64/sgx_edger8r \
		--trusted ../Enclave/Enclave.edl \
		--search-path ../Enclave \
		--search-path /opt/intel/sgxsdk/include

Enclave/Enclave_t.o: Enclave/Enclave_t.c
	gcc -m64 -O0 -g -nostdinc -fvisibility=hidden -fpie \
		-I/opt/intel/sgxsdk/include \
		-I/opt/intel/sgxsdk/include/tlibc \
		-I/opt/intel/sgxsdk/include/libcxx \
		-ffunction-sections -fdata-sections -fstack-protector-strong \
		-c $< -o $@

Enclave/Enclave.o: Enclave/Enclave.c
	gcc -m64 -O0 -g -fPIC -Wno-attribute -I/opt/intel/sgxsdk/include  -c $< -o $@

enclave.so: Enclave/Enclave_t.o Enclave/Enclave.o
	gcc  $^ -o $@ $(Enclave_Link_Flags)


enclave.signed.so: enclave.so
	/opt/intel/sgxsdk/bin/x64/sgx_sign sign -key Enclave/private_key.pem \
		-enclave enclave.so \
		-out $@ \
		-config Enclave/Enclave.config.xml

.PHOMY: clean

clean:
	arm -f app enclave.so enclave.signed.so App/Enclave_u.* Enclave/Enclave_t.* App/*.o Enclave/*.o
