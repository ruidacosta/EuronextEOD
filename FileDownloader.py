#!/usr/bin/python

import argparse
import os
import traceback
import requests
import ConfigParser
import logging
import logging.handlers
import time
import sys

from shutil import move

####################
urls = { 
    'Equities' : "https://www.euronext.com/en/popup/data/download?ml=nyx_pd_stocks&cmd=default",
    'Bonds'    : "https://www.euronext.com/en/popup/data/download?ml=nyx_pd_bonds&cmd=default",
    'Funds'    : "https://www.euronext.com/en/popup/data/download?ml=nyx_pd_etps&cmd=5",
    'ETFs'     : "https://www.euronext.com/en/popup/data/download?ml=nyx_pd_etps&cmd=4",
    'Warrants' : "https://www.euronext.com/en/popup/data/download?ml=nyx_pd_esps&cmd=default",
    'Indices'  : "https://www.euronext.com/en/popup/data/download?ml=nyx_pd_indices&cmd=default"
}

payload = {'format': 2, 'layout': 2, 'decimal_separator': 1, 'date_format': 1, 'op': 'Go', 'form_id': 'nyx_download_form'}
####################
class TlsSMTPHandler(logging.handlers.SMTPHandler):
    def emit(self, record):
        """
        Emit a record.
 
        Format the record and send it to the specified addressees.
        """
        try:
            import smtplib
            import string # for tls add this line
            try:
                from email.utils import formatdate
            except ImportError:
                formatdate = self.date_time
            port = self.mailport
            if not port:
                port = smtplib.SMTP_PORT
            smtp = smtplib.SMTP(self.mailhost, port)
            msg = self.format(record)
            msg = "From: %s\r\nTo: %s\r\nSubject: %s\r\nDate: %s\r\n\r\n%s" % (
                            self.fromaddr,
                            string.join(self.toaddrs, ","),
                            self.getSubject(record),
                            formatdate(), msg)
            if self.username:
                smtp.ehlo() # for tls add this line
                smtp.starttls() # for tls add this line
                smtp.ehlo() # for tls add this line
                smtp.login(self.username, self.password)
            smtp.sendmail(self.fromaddr, self.toaddrs, msg)
            smtp.quit()
        except (KeyboardInterrupt, SystemExit):
            raise
        except:
            self.handleError(record)
####################

def checkFile(path):
    if not (os.path.exists(path)):
        msg = "File %s not exist" % path
        raise argparse.ArgumentTypeError(msg)
    return path

def checkDirPath(path):
    if not (os.path.isdir(path)):
        msg = "%s is not a directory" % path
        raise argparse.ArgumentTypeError(msg)
    return path
    
def parseArgs():
    parser = argparse.ArgumentParser(description='Get EOD Euronext files.')
    parser.add_argument('-o', '--output', action='store', dest='output', type=checkDirPath, help='Output folder where files will be stored')
    parser.add_argument('-l', '--log_folder', action='store', dest='log_folder', type=checkDirPath)
    parser.add_argument('-n', '--log_name', action='store', dest='log_name')
    parser.add_argument('-s', '--mail_server', action='store', dest='email_srv')
    parser.add_argument('-r', '--mail_port', action='store', dest='email_port', type=int)
    parser.add_argument('-t', '--mail_to', action='store', dest='email_to')
    parser.add_argument('-f', '--mail_from', action='store', dest='email_from')
    parser.add_argument('-u', '--mail_user', action='store', dest='email_user')
    parser.add_argument('-p', '--mail_pass', action='store', dest='email_pass')
    parser.add_argument('-c', '--config_file', action='store', dest='config_file', type=checkFile, required=True) #required
    args = parser.parse_args()
    return vars(args)
                                                                                                                                                                                                                    
def readConfig(precedence):
    config = ConfigParser.RawConfigParser()
    config.read(precedence['config_file'])
    if not precedence.has_key('output') or not precedence['output']:
        precedence['output'] = config.get('Downloader', 'output')
    if not precedence.has_key('log_folder') or not precedence['log_folder']:
        precedence['log_folder'] = config.get('Logs', 'folder')
    if not precedence.has_key('log_name') or not precedence['log_name']:
        precedence['log_name'] = config.get('Logs', 'name')
    if not precedence.has_key('email_srv') or not precedence['email_srv']:
        precedence['email_srv'] = config.get('Email', 'server')
    if not precedence.has_key('email_port') or not precedence['email_port']:
        precedence['email_port'] = config.get('Email', 'port')
    if not precedence.has_key('email_to') or not precedence['email_to']:
        precedence['email_to'] = config.get('Email', 'to')
    if not precedence.has_key('email_from') or not precedence['email_from']:
        precedence['email_from'] = config.get('Email', 'from')
    if not precedence.has_key('email_user') or not precedence['email_user']:
        precedence['email_user'] = config.get('Email', 'username')
    if not precedence.has_key('email_pass') or not precedence['email_pass']:
        precedence['email_pass'] = config.get('Email', 'password')
    return precedence
    
def setLogger(log_folder,log_name=None, options=None):
    if log_name:
        logger = logging.getLogger('main')
    else:
        log_name = os.path.basename(__file__)
        logger = logging.getLogger('main')
    
    try:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
        log_name = log_name[:log_name.rindex('.')]
    except ValueError:
        pass
    
    if not os.path.isdir(log_folder):
        os.makedirs(log_folder)
    
    formatter = logging.Formatter('[%(asctime)s] : [%(levelname)-5s] : %(message)s')
    logger.setLevel(logging.DEBUG)
    
    # Set console logger
    console = logging.StreamHandler()
    console.setFormatter(formatter)
    logger.addHandler(console)
    
    # Set file logger
    if log_folder.endswith(os.path.sep):
        file_name = log_folder + log_name
    else:
        file_name = log_folder + os.path.sep + log_name
    file = logging.handlers.TimedRotatingFileHandler(filename = file_name + '.log', when = 'midnight', backupCount = 30)
    file.setFormatter(formatter)
    logger.addHandler(file)
    
    # Set Email logger
    email = TlsSMTPHandler((options['email_srv'], options['email_port']), options['email_from'], options['email_to'].split(','), log_name + '::Error found!', (options['email_user'], options['email_pass']))
    email.setFormatter(formatter)
    email.setLevel(logging.ERROR)
    logger.addHandler(email)

def GetFile(url,payload):                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
    logger = logging.getLogger('main')
    flag = False
    counter = 0
    while not flag:
        try:
            counter += 1
            r = requests.post(url,data=payload)
            filename = r.headers['content-disposition'].split('=')[1]
            logger.info('Saving %s' % filename)
            with open(filename, 'wb') as fd:
                for chunk in r.iter_content(chunk_size=128):
                    fd.write(chunk)
            flag = True
        except:
            if counter == 3:
                raise
            else:
                logger.info("Retrying...(%d)" % counter)
                time.sleep(5)
    return filename
    
def moveFileTo(file, outputFolder):
    logger = logging.getLogger('main')
    if not outputFolder.endswith(os.path.sep):
        outputFolder += os.path.sep
    if not os.path.isdir(outputFolder):
        try:
            os.makedirs(outputFolder)
        except OSError:
            msg = "Can't create directory" % outputFolder
            raise OSError(msg)
    
    filename = outputFolder + file
    counter = 2
    while os.path.exists(filename):
        try: # get name/extension from file
            ext_sep = file.rindex('.')
            name = file[:ext_sep]
            ext = file[ext_sep:]
            filename = outputFolder + name + '_' + str(counter) + ext
        except ValueError:
            filename = outputFolder + file + '_' + str(counter)
        counter += 1
    
    logger.info('Move from %s to %s' % (file, filename))
    #os.rename(file,filename)
    move(file,filename)
    
def main():
    global urls, payload
    try:
        start = time.time()
        args = parseArgs()
        options = readConfig(args)
        setLogger(options['log_folder'], options=options)
        logger = logging.getLogger('main')
        logger.info("Starting service")
        for key in urls.iterkeys():
            try:
                logger.info('Getting %s file.' % key)
                file = GetFile(urls[key],payload)
                logger.info('Moving file...')
                moveFileTo(file,options['output'])
            except:
                import traceback
                s = traceback.format_exc()
                logger.error("Exception raised on %s: %s", key, s)
        logger.info('Stopping service')
        logger.info('Execution time: %f seconds' % (time.time() - start))
        logger.info('Done. Bye')
        return 0
    except:
        import traceback
        s = traceback.format_exc()
        logger.error("Exception raised: %s" % s)
        return -1

if __name__ == '__main__':
    sys.exit(main())