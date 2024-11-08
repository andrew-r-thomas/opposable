/*
	implementing this:
	https://fidoalliance.org/specs/fido-v2.0-ps-20190130/fido-client-to-authenticator-protocol-v2.0-ps-20190130.html#authenticatorMakeCredential

	 NOTE:
	 we're gonna make sure that there's no memory allocation anywhere
	 except for one part (for now, maybe we give separate mem control to
	 different cores or something)
*/


struct CBOR_map
{
	// TODO:
};
struct PublicKeyCredentialDescriptors
{
	// TODO:
};
struct options
{
	// TODO:
};

void authenticatorMakeCredential(
	unsigned char* clientDataHash,
	int clientDataHash_len,
	char *rp,
	unsigned char* user,
	int user_len,
	struct CBOR_map* pubKeyCredParams,
	int pubKeyCredParams_len,
	// optional, len will be 0 and pointer will be null if not included
	struct PublicKeyCredentialDescriptors* excludeList,
	int excludeList_len,
	// optional, pointer will be null if not included
	struct CBOR_map* extensions,
	// optional, pointer will be null if not included
	struct options* options,
	// optional, pointer will be null and len will be 0 if not included
	unsigned char* pinAuth,
	int pinAuth_len,
	// optional, pointer will be null if not included
	unsigned int* pinProtocol
)
{
	// TODO:
	// 1.
	// 2.
	// 3.
	// 4.
	// 5.
	// 6.
	// 7.
	// 8.
	// 9.
	// 10.
	// 11.
}

void authenticatorGetAssertion()
{
	// TODO:
}

void authenticatorGetNextAssertion()
{
	// TODO:
}

void authenticatorGetInfo()
{
	// TODO:
}

void authenticatorClientPin()
{
	// TODO:
}

void authenticatorReset()
{
	// TODO:
}

