 * Copyright (c) 1999-2016 Apple Inc.
#define	VNODE2_PATH_TOKENS do {					\
	if (ARG_IS_VALID(kar, ARG_KPATH2)) {				\
		tok = au_to_path(ar->ar_arg_kpath2);			\
		kau_write(rec, tok);					\
	}								\
	if (ARG_IS_VALID(kar, ARG_VNODE2)) {				\
		tok = au_to_attr32(&ar->ar_arg_vnode2);			\
		kau_write(rec, tok);					\
		MAC_VNODE2_LABEL_TOKEN;					\
	}								\
} while (0)

		VNODE2_PATH_TOKENS;
		if (ARG_IS_VALID(kar, ARG_DATA)) {
			tok = au_to_data(AUP_HEX, ar->ar_arg_data_type,
			    ar->ar_arg_data_count, ar->ar_arg_data);
			kau_write(rec, tok);
		}
	case AUE_FMOUNT:
		if (ARG_IS_VALID(kar, ARG_FD)) {
			tok = au_to_arg32(2, "dir fd", ar->ar_arg_fd);
			kau_write(rec, tok);
		}
		if (ARG_IS_VALID(kar, ARG_FFLAGS)) {
			tok = au_to_arg32(3, "flags", ar->ar_arg_fflags);
			kau_write(rec, tok);
		}
		if (ARG_IS_VALID(kar, ARG_TEXT)) {
			tok = au_to_text(ar->ar_arg_text);
			kau_write(rec, tok);
		}
		break;
	case AUE_SETATTRLISTAT:
	case AUE_CLONEFILEAT:
		if (ARG_IS_VALID(kar, ARG_FD)) {
			tok = au_to_arg32(1, "src dir fd", ar->ar_arg_fd);
			kau_write(rec, tok);
		}
		UPATH1_VNODE1_TOKENS;
		if (ARG_IS_VALID(kar, ARG_FD2)) {
			tok = au_to_arg32(1, "dst dir fd", ar->ar_arg_fd2);
			kau_write(rec, tok);
		}
		UPATH2_TOKENS;
		if (ARG_IS_VALID(kar, ARG_VALUE32)) {
			tok = au_to_arg32(1, "flags", ar->ar_arg_value32);
			kau_write(rec, tok);
		}
		break;

	case AUE_FCLONEFILEAT:
		FD_VNODE1_TOKENS;
		if (ARG_IS_VALID(kar, ARG_FD2)) {
			tok = au_to_arg32(1, "dst dir fd", ar->ar_arg_fd2);
			kau_write(rec, tok);
		}
		UPATH2_TOKENS;
		if (ARG_IS_VALID(kar, ARG_VALUE32)) {
			tok = au_to_arg32(1, "flags", ar->ar_arg_value32);
			kau_write(rec, tok);
		}
		break;

	if (ARG_IS_VALID(kar, ARG_IDENTITY)) {
		struct au_identity_info *id = &ar->ar_arg_identity;
		tok = au_to_identity(id->signer_type, id->signing_id,
			id->signing_id_trunc, id->team_id, id->team_id_trunc,
			id->cdhash, id->cdhash_len);
		kau_write(rec, tok);
	}

 * Verify that a record is a valid BSM record. Return 1 if the
bsm_rec_verify(void *rec, int length)
	/* Used to partially deserialize the buffer */
	struct hdr_tok_partial *hdr;
	struct trl_tok_partial *trl;
	/* A record requires a complete header and trailer token */
	if (length < (AUDIT_HEADER_SIZE + AUDIT_TRAILER_SIZE)) {
	}

	hdr = (struct hdr_tok_partial*)rec;

	/* Ensure the provided length matches what the record shows */
	if ((uint32_t)length != ntohl(hdr->len)) {
		return (0);
	}

	trl = (struct trl_tok_partial*)(rec + (length - AUDIT_TRAILER_SIZE));

	/* Ensure the buffer contains what look like header and trailer tokens */
	if (((hdr->type != AUT_HEADER32) && (hdr->type != AUT_HEADER32_EX) &&
	    (hdr->type != AUT_HEADER64) && (hdr->type != AUT_HEADER64_EX)) ||
	    (trl->type != AUT_TRAILER)) {
		return (0);
	}

	/* Ensure the header and trailer agree on the length */
	if (hdr->len != trl->len) {
		return (0);
	}

	/* Ensure the trailer token has a proper magic value */
	if (ntohs(trl->magic) != AUT_TRAILER_MAGIC) {
		return (0);
	}
