#pragma once

static const char *DB_STRUCTURE_SQL_SCRIPT = R"script(
CREATE TABLE IF NOT EXISTS tags (
	tag_name TEXT NOT NULL,
	color TEXT NOT NULL,
	CONSTRAINT file_tags_pk PRIMARY KEY (tag_name)
);
CREATE TABLE IF NOT EXISTS file_tags (
	tag_name TEXT NOT NULL,
	filepath TEXT NOT NULL,
	CONSTRAINT file_tags_pk PRIMARY KEY (tag_name,filepath),
	CONSTRAINT file_tags_tags_FK FOREIGN KEY (tag_name) REFERENCES tags(tag_name) ON DELETE CASCADE ON UPDATE CASCADE
);
)script";