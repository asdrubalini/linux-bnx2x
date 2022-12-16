{
	"calls: two calls returning different map pointers for lookup (hash, array)",
	.insns = {
	/* main prog */
	BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 2),
	BPF_CALL_REL(11),
	BPF_JMP_IMM(BPF_JA, 0, 0, 1),
	BPF_CALL_REL(12),
	BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
	BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
	BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),
	BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
	BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, offsetof(struct test_val, foo)),
	BPF_MOV64_IMM(BPF_REG_0, 1),
	BPF_EXIT_INSN(),
	/* subprog 1 */
	BPF_LD_MAP_FD(BPF_REG_0, 0),
	BPF_EXIT_INSN(),
	/* subprog 2 */
	BPF_LD_MAP_FD(BPF_REG_0, 0),
	BPF_EXIT_INSN(),
	},
	.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	.fixup_map_hash_48b = { 13 },
	.fixup_map_array_48b = { 16 },
	.result = ACCEPT,
	.retval = 1,
},
{
	"calls: two calls returning different map pointers for lookup (hash, map in map)",
	.insns = {
	/* main prog */
	BPF_JMP_IMM(BPF_JNE, BPF_REG_1, 0, 2),
	BPF_CALL_REL(11),
	BPF_JMP_IMM(BPF_JA, 0, 0, 1),
	BPF_CALL_REL(12),
	BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
	BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
	BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
	BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),
	BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 2),
	BPF_ST_MEM(BPF_DW, BPF_REG_0, 0, offsetof(struct test_val, foo)),
	BPF_MOV64_IMM(BPF_REG_0, 1),
	BPF_EXIT_INSN(),
	/* subprog 1 */
	BPF_LD_MAP_FD(BPF_REG_0, 0),
	BPF_EXIT_INSN(),
	/* subprog 2 */
	BPF_LD_MAP_FD(BPF_REG_0, 0),
	BPF_EXIT_INSN(),
	},
	.prog_type = BPF_PROG_TYPE_SCHED_CLS,
	.fixup_map_in_map = { 16 },
	.fixup_map_array_48b = { 13 },
	.result = REJECT,
	.errstr = "only read from bpf_array is supported",
},
{
	"cond: two branches returning different map pointers for lookup (tail, tail)",
	.insns = {
	BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
		    offsetof(struct __sk_buff, mark)),
	BPF_JMP_IMM(BPF_JNE, BPF_REG_6, 0, 3),
	BPF_LD_MAP_FD(BPF_REG_2, 0),
	BPF_JMP_IMM(BPF_JA, 0, 0, 2),
	BPF_LD_MAP_FD(BPF_REG_2, 0),
	BPF_MOV64_IMM(BPF_REG_3, 7),
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_tail_call),
	BPF_MOV64_IMM(BPF_REG_0, 1),
	BPF_EXIT_INSN(),
	},
	.fixup_prog1 = { 5 },
	.fixup_prog2 = { 2 },
	.result_unpriv = REJECT,
	.errstr_unpriv = "tail_call abusing map_ptr",
	.result = ACCEPT,
	.retval = 42,
},
{
	"cond: two branches returning same map pointers for lookup (tail, tail)",
	.insns = {
	BPF_LDX_MEM(BPF_W, BPF_REG_6, BPF_REG_1,
		    offsetof(struct __sk_buff, mark)),
	BPF_JMP_IMM(BPF_JEQ, BPF_REG_6, 0, 3),
	BPF_LD_MAP_FD(BPF_REG_2, 0),
	BPF_JMP_IMM(BPF_JA, 0, 0, 2),
	BPF_LD_MAP_FD(BPF_REG_2, 0),
	BPF_MOV64_IMM(BPF_REG_3, 7),
	BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_tail_call),
	BPF_MOV64_IMM(BPF_REG_0, 1),
	BPF_EXIT_INSN(),
	},
	.fixup_prog2 = { 2, 5 },
	.result_unpriv = ACCEPT,
	.result = ACCEPT,
	.retval = 42,
},
