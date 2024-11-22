#pragma once

const char *DB_STRUCTURE_SQL_SCRIPT = R"script(
CREATE TABLE IF NOT EXISTS tags (
	tag_name TEXT NOT NULL,
	CONSTRAINT file_tags_pk PRIMARY KEY (tag_name)
);
CREATE TABLE IF NOT EXISTS tag_categories (
	category_name TEXT NOT NULL,
	tag_name TEXT NOT NULL,
	CONSTRAINT tag_categories_pk PRIMARY KEY (category_name,tag_name),
	CONSTRAINT tag_categories_tags_FK FOREIGN KEY (tag_name) REFERENCES tags(tag_name) ON DELETE CASCADE ON UPDATE CASCADE
);
CREATE TABLE IF NOT EXISTS file_tags (
	tag_name TEXT NOT NULL,
	filepath TEXT NOT NULL,
	CONSTRAINT file_tags_pk PRIMARY KEY (tag_name,filepath),
	CONSTRAINT file_tags_tags_FK FOREIGN KEY (tag_name) REFERENCES tags(tag_name) ON DELETE CASCADE ON UPDATE CASCADE
);
)script";