from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
from email.MIMEImage import MIMEImage
import smtplib

strFrom = "mtgp02@gmail.com"
strTo = "mtgp02@gmail.com"

msgRoot = MIMEMultipart('related')
msgRoot['Subject'] = "Subject Test"
msgRoot['From'] = strFrom
msgRoot['To'] = strTo
msgRoot.preamble = 'Multipart message in MIME format'

msgAlternative = MIMEMultipart('alternative')
msgRoot.attach(msgAlternative)

msgText = MIMEText('Plain text message here.')
msgAlternative.attach(msgText)

fp = open('/home/iii/Desktop/ih20/TEMPLATE/email/ERF.html','rt')
#msgText = MIMEText('<b>HTML MIME Message here</b><img src="cid:image1">','html')
msgText = MIMEText(fp.read(),'html')
fp.close()
msgAlternative.attach(msgText)

fp = open('/mswin/Image1.jpg','rb')
msgImage = MIMEImage(fp.read())
fp.close()

msgImage.add_header('Content-ID','<image1>')
msgRoot.attach(msgImage)


try:
	smtp = smtplib.SMTP('smtp.gmail.com',587)
	smtp.ehlo()
	smtp.starttls()
#	smtp.connect()
	smtp.login('mtgp02','klatu,berata,niktu!1024')
	smtp.sendmail(strFrom,strTo,msgRoot.as_string())
	smtp.quit()

except:
	print("Error")
