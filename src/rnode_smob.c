/* Represents an rnode as a SMOB (Guile primitive) */

#include <libguile.h>

#include "rnode.h"


static scm_t_bits rnode_tag;

// TODO: can probably use NULL here?
SCM mark_rnode(SCM rnode_smob)
{
	/* nothing to do: rnode_smob holds no SCM. */
	return SCM_BOOL_F;
}

// TODO: cf above
size_t free_rnode(SCM rnode_smob)
{
	return 0;
}

void init_scm_rnode()
{
	rnode_tag = scm_make_smob_type("node", sizeof(struct rnode));
	scm_set_smob_mark(rnode_tag, mark_rnode);
	scm_set_smob_free(rnode_tag, free_rnode);
	// scm_set_smob_print(rnode_tag, print_rnode);
}

SCM rnode_smob(struct rnode *node)
{
	SCM smob;
	SCM_NEWSMOB (smob, rnode_tag, node);

	return smob;
}

SCM rnode_smob_label(SCM node_smob, SCM label)
{

	if (SCM_UNDEFINED == node_smob)
		return SCM_UNDEFINED;

	struct rnode *node;
	node = (struct rnode*) SCM_SMOB_DATA (node_smob);

	if (SCM_UNDEFINED != label) { 
		size_t buffer_length;	/* storage for label */

		if (scm_is_number (label))
			label = scm_number_to_string(label, SCM_UNDEFINED);	

		buffer_length = scm_c_string_length(label);
		char *buffer = calloc(buffer_length + 1, 'c');	/* +1: '\0' */
		scm_to_locale_stringbuf(label, buffer, buffer_length);
		buffer[buffer_length] = '\0';

		/* Set the allocated buffer as the node's length-as-string */
		free(node->label);
		node->label = buffer;
	}
	
	// TODO This is inefficient!
	char *c_label = strdup(node->label);
	/* no need to free label (see scm_take_locale_string()) */

	return label = scm_take_locale_string(c_label);
}

SCM rnode_smob_parent(SCM node_smob)
{
	if (SCM_UNDEFINED == node_smob)
		return SCM_UNDEFINED;

	struct rnode *node;
	node = (struct rnode*) SCM_SMOB_DATA (node_smob);
	if (is_root(node))
		return SCM_UNDEFINED;

	return rnode_smob(node->parent);
}
