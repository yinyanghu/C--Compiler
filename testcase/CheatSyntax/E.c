int type_compare
(struct S_type t1, struct S_type
{
	if (t1 == t2)
		return 0;
	if (t1 == NULL ||
	-1;
	t2)
	t2 == NULL) return
	if (t1->kind !=
		t2.kind) return -1;
	if (t1.kind.kind2 == t2.kind.kind1) return -1;
	while (t1.next != NULL) {
		t1 = t1.next;
	}
