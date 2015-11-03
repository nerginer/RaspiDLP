import logging
from pubsub import pub

logger = logging.getLogger('DLP_log')

hdlr = logging.FileHandler('DLP.log')

formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')

hdlr.setFormatter(formatter)

logger.addHandler(hdlr) 

logger.setLevel(logging.DEBUG)



def listener_Func(arg1, arg2):
    if (arg1=='error'):
        logger.error(arg2)
    if (arg1=='debug'):
        logger.debug(arg2)
    if (arg1=='warning'):
        logger.warning(arg2)
    if (arg1=='info'):
        logger.info(arg2)

# ------------ register listener ------------------

pub.subscribe(listener_Func, 'Log')

# usage
# pub.sendMessage('Log', arg1='error', arg2='Can not open USB Port')
