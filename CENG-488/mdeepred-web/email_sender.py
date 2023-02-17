import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders
import os
cwd = os.getcwd()


def send_mail(adress):

    mail_content = '''Hello,
    Your drug - target interaction request has been resolved, both bioassay results and prediction results are added in attachment. 
    Have a nice day,
    MDeePred-Web
    '''
    # The mail addresses and password
    sender_address = 'anilutku600@gmail.com'
    sender_pass = 'wlufvivnnibrwrel'
    receiver_address = adress
    # Setup the MIME
    message = MIMEMultipart()
    message['From'] = sender_address
    message['To'] = receiver_address
    message['Subject'] = 'MDeePred-Web Test Results'
    # The subject line
    # The body and the attachments for the mail
    message.attach(MIMEText(mail_content, 'plain'))
    sent_files_path = "{}mdeepred-web/sent_files".format(cwd.split("mdeepred-web")[0])
    attach_file_path = "{}/{}.csv".format(sent_files_path, adress)
    attach_file = open(attach_file_path, 'rb')  # Open the file as binary mode
    payload = MIMEBase('application', 'octate-stream', Name="interaction_values.csv")
    payload.set_payload(attach_file.read())
    encoders.encode_base64(payload)  # encode the attachment
    # add payload header with filename
    payload.add_header('Content-Decomposition', 'attachment', filename=attach_file_path)
    message.attach(payload)
    # Create SMTP session for sending the mail
    session = smtplib.SMTP('smtp.gmail.com', 587)  # use gmail with port
    session.starttls()  # enable security
    session.login(sender_address, sender_pass)  # login with mail_id and password
    text = message.as_string()
    session.sendmail(sender_address, receiver_address, text)
    session.quit()
    print('Mail Sent')
