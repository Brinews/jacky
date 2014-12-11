import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.io.Reader;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.LinkedList;

import org.apache.commons.mail.EmailAttachment;
import org.apache.commons.mail.EmailException;
import org.apache.commons.mail.MultiPartEmail;

class hqzs_mail {
	
	private String mailst = "./conf/mail.lst";
	private List<String> mlist = null;

	public hqzs_mail()
	{
		mlist = new LinkedList<String>();
		ReadMailList();
	}

	public void ReadMailList()
	{
		File fd = null;
		Reader in = null;
		try {
			fd = new File(mailst);
			in = new FileReader(fd);
		} catch (FileNotFoundException fnfe) {
			fnfe.printStackTrace();
		}

		mlist.clear();
		mlist.add("10848218@pku.edu.cn");
		String mailAddr = "";
		int c;

		try {
			while ((c = in.read()) > 0) {
				mailAddr += (char) c;
				if (c == '\n') {
					if (mailAddr.indexOf("@") > 0) {
						mlist.add(mailAddr.trim());
					} else {
						System.out.println("Wrong Mail Address:" + mailAddr);
					}
					mailAddr = "";
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		if (mlist.size() == 0) {
			System.err.println("No mail address found, Please configure the ./conf/mail.lst file");
		}
	}

	public void SendToAll()
	{
		for (int i = 0; i < mlist.size(); i++) {
			String mailAddr = mlist.get(i);
			System.out.println(mailAddr + "==" + mailAddr.split("@")[0]);
			send(mailAddr, mailAddr.split("@")[0], "Week Report", GlobalConf.mailCont);
		}
	}

	public void send(String toAddr, String toName, 
			String subject, String body) {
		try {
			/* create attachment */
			EmailAttachment attach = new EmailAttachment();
			attach.setPath(GlobalConf.pdfFileName);
			attach.setDisposition(EmailAttachment.ATTACHMENT);
			attach.setDescription("Analysis PDF file");
			attach.setName(GlobalConf.pdfFileName);


			/* 登陆发送邮件服务器 */
			MultiPartEmail email = new MultiPartEmail();
			email.setHostName("smtp.126.com");
			email.addTo(toAddr, toName);
			email.setFrom("wrongkill@126.com", "wrongkill");
			email.setSubject(subject);
			email.setAuthentication("wrongkill", "cache123");
			email.setMsg(body); 

			/* add attachment */
			email.attach(attach);

			/* send */
			email.send();

		} catch(EmailException e) {
			e.printStackTrace();
		}
	}
}
