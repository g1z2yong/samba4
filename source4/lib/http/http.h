/*
   Unix SMB/CIFS implementation.

   HTTP library

   Copyright (C) Zentyal S.L. 2013 <scabrero@zentyal.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _HTTP_H_
#define _HTTP_H_

#include <limits.h>

struct tevent_context;
struct tstream_context;
struct http_header;
struct http_uri;

/* Response codes */
#define HTTP_OK					200		/**< request completed ok */
#define HTTP_NOCONTENT			204		/**< request does not have content */
#define HTTP_MOVEPERM			301		/**< the uri moved permanently */
#define HTTP_MOVETEMP			302		/**< the uri moved temporarily */
#define HTTP_NOTMODIFIED		304		/**< page was not modified from last */
#define HTTP_BADREQUEST			400		/**< invalid http request was made */
#define HTTP_NOTFOUND			404		/**< could not find content for uri */
#define HTTP_BADMETHOD			405 	/**< method not allowed for this uri */
#define HTTP_ENTITYTOOLARGE		413		/**<  */
#define HTTP_EXPECTATIONFAILED	417		/**< we can't handle this expectation */
#define HTTP_INTERNAL           500     /**< internal error */
#define HTTP_NOTIMPLEMENTED     501     /**< not implemented */
#define HTTP_SERVUNAVAIL		503		/**< the server is not available */

#define HTTP_MAX_HEADER_SIZE 	UINT_MAX

/**
 * A request object can represent either a request or a reply
 * */
enum http_request_kind { HTTP_REQUEST, HTTP_RESPONSE };
enum http_cmd_type {
	HTTP_REQ_GET     = 1 << 0,
	HTTP_REQ_POST    = 1 << 1,
	HTTP_REQ_HEAD    = 1 << 2,
	HTTP_REQ_PUT     = 1 << 3,
	HTTP_REQ_DELETE  = 1 << 4,
	HTTP_REQ_OPTIONS = 1 << 5,
	HTTP_REQ_TRACE	 = 1 << 6,
	HTTP_REQ_CONNECT = 1 << 7,
	HTTP_REQ_PATCH	 = 1 << 8,
	HTTP_REQ_RPC_IN_DATA  = 1 << 9,
	HTTP_REQ_RPC_OUT_DATA = 1 << 10,
};

struct http_request {
	struct tevent_context *ev;

	/* address of the remote host and the port connection came from */
	char *remote_host;
	uint16_t remote_port;

	/* cache of the host name for http_request_get_host */
	char *host_cache;

	char *uri;							/* URI after HTTP request was parsed */
	struct http_uri *uri_elems;			/* uri elements */

	struct http_header *request_headers;
	struct http_header *response_headers;

	char *response_phrase;

	enum http_request_kind kind;
	enum http_cmd_type type;

	char major;							/* HTTP version major number */
	char minor;							/* HTTP version minor number */

	size_t headers_size;
	size_t body_size;

	DATA_BLOB *output_buffer;			/* data for request body */
	DATA_BLOB *input_buffer;			/* response read data */

	DATA_BLOB *body;
	int response_code;		/* HTTP Response code */
	char *response_code_line;	/* Readable response */
};

struct tevent_req *http_send_request_send(TALLOC_CTX *mem_ctx,
		struct tevent_context *ev,
		struct tstream_context *stream,
		struct tevent_queue *send_queue,
		const char *remote_host,
		enum http_cmd_type type,
		const char *uri,
		DATA_BLOB *data);

struct tevent_req *http_read_response_send(TALLOC_CTX *mem_ctx,
		struct tevent_context *ev,
		struct tstream_context *stream);
#endif /* _HTTP_H_ */