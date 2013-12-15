//void draw_buffer(
//	GLsizei indices, GLuint element_buffer,
//	GLuint position_buffer, GLuint color_buffer, GLuint normal_buffer, GLuint texel_buffer,
//	GLuint position_loc, GLuint color_loc, GLuint normal_loc, GLuint texel_loc)
//{
//	glEnableVertexAttribArray(position_loc);
//	glEnableVertexAttribArray(color_loc);
//	glEnableVertexAttribArray(normal_loc);
//	glEnableVertexAttribArray(texel_loc);
//	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
//	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
//	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
//	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, texel_buffer);
//	glVertexAttribPointer(texel_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
//	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glDisableVertexAttribArray(position_loc);
//	glDisableVertexAttribArray(color_loc);
//	glDisableVertexAttribArray(normal_loc);
//	glDisableVertexAttribArray(texel_loc);
//}