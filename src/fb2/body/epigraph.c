#include "../fb2_chunks.h"

int parse_epigraph(xmlNode* node, GtkTextBuffer* text_buff, GtkTextIter* text_buff_end)
{
	assert(node != NULL);
	assert(text_buff != NULL);
	assert(text_buff_end != NULL);

	node = node->children;

	GtkTextTag* epigraph_tag		= GLOBAL_FB2_READER.epigraph_tag;

	GtkTextMark* start_tag_mark		= gtk_text_buffer_create_mark(text_buff, NULL, text_buff_end, true);

	while(node != NULL)
	{
		if(node->type == XML_ELEMENT_NODE)
		{
			if(strcmp((char*)node->name, "p") == 0)
				parse_p(node, text_buff, text_buff_end);
			else if(strcmp((char*)node->name, "poem") == 0)
				parse_poem(node, text_buff, text_buff_end);
			else if(strcmp((char*)node->name, "cite") == 0)
				parse_cite(node, text_buff, text_buff_end);
			else if(strcmp((char*)node->name, "empty-line") == 0)
				gtk_text_buffer_insert(text_buff, text_buff_end, "\n", -1);
			else if(strcmp((char*)node->name, "text-author") == 0)
				parse_text_autor(node, text_buff, text_buff_end);
		}

		node = node->next;
	}

	GtkTextIter start_tag_iter;
	gtk_text_buffer_get_iter_at_mark(text_buff, &start_tag_iter, start_tag_mark);
	gtk_text_buffer_delete_mark(text_buff, start_tag_mark);
	//g_object_unref(G_OBJECT(start_tag_mark));
	gtk_text_buffer_apply_tag(text_buff, epigraph_tag, &start_tag_iter, text_buff_end);

	return 0;
}
