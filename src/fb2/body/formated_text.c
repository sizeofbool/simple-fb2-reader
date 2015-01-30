#include "../fb2_chunks.h"

int parse_p(xmlNode* node, GtkTextBuffer* text_buff, GtkTextIter* text_buff_end)
{
	assert(node != NULL);
	assert(text_buff != NULL);
	assert(text_buff_end != NULL);

	node = node->children;

	gtk_text_buffer_insert(text_buff, text_buff_end, "\t", -1);
	parse_formated_text(node, text_buff, text_buff_end);
	gtk_text_buffer_insert(text_buff, text_buff_end, "\n", -1);

	return 0;
}


int parse_formated_text(xmlNode* node, GtkTextBuffer* text_buff, GtkTextIter* text_buff_end)
{
	assert(node != NULL);
	assert(text_buff != NULL);
	assert(text_buff_end != NULL);

	GtkTextTag* strong_tag			= GLOBAL_FB2_READER.strong_tag;
	GtkTextTag* emphasis_tag		= GLOBAL_FB2_READER.emphasis_tag;
	GtkTextTag* strikethrough_tag	= GLOBAL_FB2_READER.strikethrough_tag;
	GtkTextTag* sub_tag				= GLOBAL_FB2_READER.sub_tag;
	GtkTextTag* sup_tag				= GLOBAL_FB2_READER.sup_tag;
	GtkTextTag* code_tag			= GLOBAL_FB2_READER.code_tag;

	GtkTextTag* tag = NULL;

	while(node != NULL)
	{
		tag = NULL;

		if(strcmp((char*)node->name, "text") == 0)					// обычный текст
			gtk_text_buffer_insert(text_buff, text_buff_end, (char*)node->content, -1);
		else if(strcmp((char*)node->name, "strong") == 0)			// жирный
			tag = strong_tag;
		else if(strcmp((char*)node->name, "emphasis") == 0)			// курсив
			tag = emphasis_tag;
		else if(strcmp((char*)node->name, "a") == 0)				// ссылка
			parse_link(node, text_buff, text_buff_end);
		else if(strcmp((char*)node->name, "strikethrough") == 0)	// зачеркнутый
			tag = strikethrough_tag;
		else if(strcmp((char*)node->name, "sub") == 0)				// нижний индекс
			tag = sub_tag;
		else if(strcmp((char*)node->name, "sup") == 0)				// верхний индекс
			tag = sup_tag;
		else if(strcmp((char*)node->name, "code") == 0)				// код, моноширинный шрифт
			tag = code_tag;
		else if(strcmp((char*)node->name, "image") == 0)			// картинка
			parse_image(node, text_buff, text_buff_end);
		else if(strcmp((char*)node->name, "style") == 0)			// стилевое оформление
			print_unsupported_tag("style");



		if(tag != NULL)
		{
			GtkTextMark* start_tag_mark = gtk_text_buffer_create_mark(text_buff, NULL, text_buff_end, true);

			parse_formated_text(node->children, text_buff, text_buff_end);

			GtkTextIter start_tag_iter;
			gtk_text_buffer_get_iter_at_mark(text_buff, &start_tag_iter, start_tag_mark);
			gtk_text_buffer_delete_mark(text_buff, start_tag_mark);
			//g_object_unref(G_OBJECT(start_tag_mark));
			gtk_text_buffer_apply_tag(text_buff, tag, &start_tag_iter, text_buff_end);

		}

		node = node->next;
	}
	return 0;
}
