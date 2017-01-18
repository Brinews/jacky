#include <stdlib.h>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <stdio.h>
#include <exception>

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

#include "FCgiIO.h"
#include "fcgi_stdio.h"

#include "consrv.h"

using namespace cgicc;

int main(int argc, char **argv) {

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        std::string username, password, nickname, picname;
        bool is_input_ok = false;

        FCgiIO io(request);
        Cgicc cgi(&io);

        do {
            try {

                form_iterator fvalue1 = cgi.getElement("username");
                if(!fvalue1->isEmpty() && fvalue1 != (*cgi).end()) {
                    username = **fvalue1;
                } else {
                    break;
                }

                form_iterator fvalue2 = cgi.getElement("password");
                if(!fvalue2->isEmpty() && fvalue2 != (*cgi).end()) {
                    password = **fvalue2;
                } else {
                    break;
                }

                form_iterator fvalue3 = cgi.getElement("nickname");
                if(!fvalue3->isEmpty() && fvalue3 != (*cgi).end()) {
                    nickname = **fvalue3;
                } else {
                    break;
                }

                form_iterator fvalue4 = cgi.getElement("picname");
                if(!fvalue4->isEmpty() && fvalue4 != (*cgi).end()) {
                    picname = **fvalue4;
                } else {
                    break;
                }

                is_input_ok = true;

            } catch (const std::exception&) {
            }
        } while (0);

        Json::Value data;
        char buf[MSGLEN];

        int sock = connectServer();
		data["name"] = username;
		data["pwd"] = password;
		data["type"] = "set";
		data["nickname"] = nickname;
		data["picname"] = picname;

		/* send login info */
		memset(buf, 0, sizeof(buf));
		strcpy(buf, data.toStyledString().c_str());
		send(sock, buf, MSGLEN, 0);

		/* recv login status */
		int numbytes = recv(sock, buf, MSGLEN, 0);

		Json::Reader reader;
		Json::Value val;

		if (reader.parse(buf, val)) {
			std::string status = val["status"].asString();

			std::string ret = "status=";
			ret += val["status"].asString();
			ret += "&username=";
			ret += val["name"].asString();
			ret += "&password=";
			ret += val["pwd"].asString();
			ret += "&nickname=";
			ret += val["nickname"].asString();
			ret += "&picname=";
			ret += val["picname"].asString();

			io << "Content-type: text/html\r\n" << "\r\n"
				<< ret
				//<< val.toStyledString()
				<< std::endl;
		}

        close(sock);

        FCGX_Finish_r(&request);
    }

    return 0;
}
