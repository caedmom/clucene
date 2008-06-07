/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
* 
* Distributable under the terms of either the Apache License (Version 2.0) or 
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#ifndef _lucene_index_FieldsWriter_
#define _lucene_index_FieldsWriter_

#if defined(_LUCENE_PRAGMA_ONCE)
# pragma once
#endif

#include "CLucene/util/VoidMap.h"
#include "CLucene/store/Directory.h"
#include "CLucene/store/RAMDirectory.h"
#include "CLucene/store/IndexOutput.h"
#include "CLucene/document/Document.h"
#include "CLucene/document/Field.h"
#include "FieldInfos.h"

CL_NS_DEF(index)
class FieldsWriter :LUCENE_BASE{
private:
	FieldInfos* fieldInfos;

	CL_NS(store)::IndexOutput* fieldsStream;
	CL_NS(store)::IndexOutput* indexStream;

	bool doClose;

public:
	LUCENE_STATIC_CONSTANT(uint8_t, FIELD_IS_TOKENIZED = 0x1);
	LUCENE_STATIC_CONSTANT(uint8_t, FIELD_IS_BINARY = 0x2);
	LUCENE_STATIC_CONSTANT(uint8_t, FIELD_IS_COMPRESSED = 0x4);

	FieldsWriter(CL_NS(store)::Directory* d, const char* segment, FieldInfos* fn);
	FieldsWriter(CL_NS(store)::IndexOutput* fdx, CL_NS(store)::IndexOutput* fdt, FieldInfos* fn);
	~FieldsWriter();

	// Writes the contents of buffer into the fields stream
	// and adds a new entry for this document into the index
	// stream.  This assumes the buffer was already written
	// in the correct fields format.
	void flushDocument(int32_t numStoredFields, CL_NS(store)::RAMIndexOutput* buffer) {
		indexStream->writeLong(fieldsStream->getFilePointer());
		fieldsStream->writeVInt(numStoredFields);
		buffer->writeTo(fieldsStream);
	}

	void flush() {
      indexStream->flush();
      fieldsStream->flush();
    }

	/** Bulk write a contiguous series of documents.  The
	*  lengths array is the length (in bytes) of each raw
	*  document.  The stream IndexInput is the
	*  fieldsStream from which we should bulk-copy all
	*  bytes. */
	void addRawDocuments(CL_NS(store)::IndexInput* stream, const int32_t* lengths, const int32_t numDocs) {
		int64_t position = fieldsStream->getFilePointer();
		const int64_t start = position;
		for(int32_t i=0;i<numDocs;i++) {
			indexStream->writeLong(position);
			position += lengths[i];
		}
		fieldsStream->copyBytes(stream, position-start);
		CND_CONDITION(fieldsStream->getFilePointer() == position,"fieldsStream->getFilePointer() != position");
	}


	inline void writeField(FieldInfo* fi, CL_NS(document)::Field* field);

	void close();

	void addDocument(CL_NS(document)::Document* doc);
};
CL_NS_END
#endif
