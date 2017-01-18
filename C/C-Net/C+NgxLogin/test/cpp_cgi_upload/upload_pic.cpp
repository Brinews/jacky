#include "fcgi_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include <jsoncpp/json/json.h>
#include <stdio.h>

int main(void) {
    int count = 0;
    bool up_pic_succ = false;
	char name[64] = {0};
	std::string pic_path = "/home/larry/";

    while (FCGI_Accept() >= 0) {
        do {
            char * method = getenv("REQUEST_METHOD");
            if (!strcmp(method, "POST")) {

                uuid_t uuid;
                uuid_generate(uuid);
                snprintf(name,
                        sizeof(name),
                        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
                        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);

				pic_path = "/home/larry/";
                pic_path += name;

                FILE *fp = fopen(pic_path.c_str(), "w");
                if (fp == NULL) {
                    break;
                }

                int ilen = atoi(getenv("CONTENT_LENGTH"));
                char buf[4096];
                while (ilen > 0) {
                    int rlen = 4096 < ilen ? 4096 : ilen;
                    fread(buf, rlen, 1, stdin);
                    fwrite(buf, rlen, 1, fp);
                    ilen -= rlen;
                }

                fclose(fp);
                up_pic_succ = true;
            }
        } while (0);

        Json::Value data;
		std::string ret = "failed";

        if (up_pic_succ) {
        	ret = pic_path;
        }

		printf("Content-type: text/html\r\n"
				"\r\n"
				"%s\n", ret.c_str());
    }

    return 0;
}
