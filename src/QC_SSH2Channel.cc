/*
  SSH2Channel.cc

  libssh2 ssh2 channel integration into qore

  Copyright 2010 Wolfgang Ritzinger

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SSH2Channel.h"

qore_classid_t CID_SSH2_CHANNEL;
QoreClass *QC_SSH2CHANNEL;

void SSH2CHANNEL_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink) {
   xsink->raiseException("SSH2CHANNEL-CONSTRUCTOR-ERROR", "this class cannot be directly constructed but is created from methods in the SSH2Client class (ex: SSH2Client::openSessionChannel())");
}

// no copy allowed
void SSH2CHANNEL_copy(QoreObject *self, QoreObject *old, SSH2Channel *c, ExceptionSink *xsink) {
  xsink->raiseException("SSH2CHANNEL-COPY-ERROR", "copying SSH2Channel objects is not supported");
}

static void SSH2CHANNEL_destructor(QoreObject *self, SSH2Channel *c, ExceptionSink *xsink) {
   c->destructor();
   c->deref();
}

// SSH2Channel::setenv(string $var, string $value, softint $timeout_ms = -1) returns nothing
// SSH2Channel::setenv(string $var, string $value, date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_setenv(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   const QoreStringNode *name = HARD_QORE_STRING(params, 0);
   const QoreStringNode *value = HARD_QORE_STRING(params, 1);

   c->setenv(name->getBuffer(), value->getBuffer(), getMsMinusOneInt(get_param(params, 2)), xsink);
   return 0;
}

// SSH2Channel::requestPty(string $term = "vanilla", string $modes = "", softint $width = LIBSSH2_TERM_WIDTH, softint $height = LIBSSH2_TERM_HEIGHT, softint $width_px = LIBSSH2_TERM_WIDTH_PX, softint $height_px = LIBSSH2_TERM_HEIGHT_PX, softint $timeout_ms = -1) returns nothing
// SSH2Channel::requestPty(string $term = "vanilla", string $modes = "", softint $width = LIBSSH2_TERM_WIDTH, softint $height = LIBSSH2_TERM_HEIGHT, softint $width_px = LIBSSH2_TERM_WIDTH_PX, softint $height_px = LIBSSH2_TERM_HEIGHT_PX, date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_requestPty(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   static const char *SSH2CHANNEL_REQUESTPTY_ERR = "SSH2CHANNEL-REQUESTPTY-ERROR";

   const QoreStringNode *term = HARD_QORE_STRING(params, 0);
   const QoreStringNode *modes = HARD_QORE_STRING(params, 1);
   int width = HARD_QORE_INT(params, 2);
   if (width < 0) {
      xsink->raiseException(SSH2CHANNEL_REQUESTPTY_ERR, "terminal width given as the optional third argument to SSH2Channel::requestPty() must be non-negative; value given: %d", width);
      return 0;
   }
   int height = HARD_QORE_INT(params, 3);
   if (height < 0) {
      xsink->raiseException(SSH2CHANNEL_REQUESTPTY_ERR, "terminal height given as the optional fourth argument to SSH2Channel::requestPty() must be non-negative; value given: %d", height);
      return 0;
   }
   int width_px = HARD_QORE_INT(params, 4);
   if (width_px < 0) {
      xsink->raiseException(SSH2CHANNEL_REQUESTPTY_ERR, "terminal pixel width given as the optional fifth argument to SSH2Channel::requestPty() must be non-negative; value given: %d", width_px);
      return 0;
   }
   int height_px = HARD_QORE_INT(params, 5);
   if (height_px < 0) {
      xsink->raiseException(SSH2CHANNEL_REQUESTPTY_ERR, "terminal pixel height given as the optional sixth argument to SSH2Channel::requestPty() must be non-negative; value given: %d", height_px);
      return 0;
   }

   c->requestPty(xsink, *term, *modes, width, height, width_px, height_px, getMsMinusOneInt(get_param(params, 6)));
   return 0;
}

// SSH2Channel::shell(softint $timeout_ms = -1) returns nothing
// SSH2Channel::shell(date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_shell(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->shell(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::eof() returns bool
AbstractQoreNode *SSH2CHANNEL_eof(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   bool b = c->eof(xsink);
   return *xsink ? get_bool_node(b) : 0;
}

// SSH2Channel::sendEof(softint $timeout_ms = -1) returns nothing
// SSH2Channel::sendEof(date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_sendEof(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->sendEof(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::waitEof(softint $timeout_ms = -1) returns nothing
// SSH2Channel::waitEof(date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_waitEof(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->waitEof(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::exec(string $command, softint $timeout_ms = -1) returns nothing
// SSH2Channel::exec(string $command, date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_exec(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   const QoreStringNode *command = HARD_QORE_STRING(params, 0);
   c->exec(command->getBuffer(), getMsMinusOneInt(get_param(params, 1)), xsink);
   return 0;
}

// SSH2Channel::subsystem(string $command, softint $timeout_ms = -1) returns nothing
// SSH2Channel::subsystem(string $command, date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_subsystem(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   const QoreStringNode *command = HARD_QORE_STRING(params, 0);
   c->subsystem(command->getBuffer(), getMsMinusOneInt(get_param(params, 1)), xsink);
   return 0;
}

// SSH2Channel::read(softint $stream_id = 0, softint $timeout_ms = 10000) returns string
// SSH2Channel::read(softint $stream_id = 0, date $timeout) returns string
AbstractQoreNode *SSH2CHANNEL_read(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   int stream = HARD_QORE_INT(params, 0);
   if (stream < 0) {
      xsink->raiseException("SSH2CHANNEL-READ-ERROR", "expecting non-negative integer for stream id as optional first argument to SSH2Channel::read([streamid], [timeout_ms]), got %d instead; use 0 for stdin, 1 for stderr");
      return 0;
   }
   return c->read(xsink, stream, getMsTimeoutWithDefault(get_param(params, 1), DEFAULT_TIMEOUT_MS));
}

// SSH2Channel::readBinary(softint $stream_id = 0, softint $timeout_ms = 10000) returns binary
// SSH2Channel::readBinary(softint $stream_id = 0, date $timeout) returns binary
AbstractQoreNode *SSH2CHANNEL_readBinary(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   int stream = HARD_QORE_INT(params, 0);
   if (stream < 0) {
      xsink->raiseException("SSH2CHANNEL-READBINARY-ERROR", "expecting non-negative integer for stream id as optional first argument to SSH2Channel::readBinary([streamid], [timeout_ms]), got %d instead; use 0 for stdin, 1 for stderr");
      return 0;
   }
   return c->readBinary(xsink, stream, getMsTimeoutWithDefault(get_param(params, 1), DEFAULT_TIMEOUT_MS));
}

// SSH2Channel::readBlock(softint $size, softint $stream_id = 0, softint $timeout_ms = -1) returns string
// SSH2Channel::readBlock(softint $size, softint $stream_id = 0, date $timeout) returns string
AbstractQoreNode *SSH2CHANNEL_readBlock(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   static const char *SSH2CHANNEL_READBLOCK_ERROR = "SSH2CHANNEL-READBLOCK-ERROR";
   int64 size = HARD_QORE_INT(params, 0);
   if (size <= 0) {
      xsink->raiseException(SSH2CHANNEL_READBLOCK_ERROR, "expecting a positive size for the block size to read, got %lld instead; use SSH2Channel::read() to read available data without a block size", size);
      return 0;
   }
   int stream = HARD_QORE_INT(params, 1);
   if (stream < 0) {
      xsink->raiseException(SSH2CHANNEL_READBLOCK_ERROR, "expecting non-negative integer for stream id as optional second argument to SSH2Channel::readBlock(blocksize, [streamid], [timeout_ms]), got %d instead; use 0 for stdin, 1 for stderr");
      return 0;
   }

   return c->read(size, stream, getMsMinusOneInt(get_param(params, 2)), xsink);
}

// SSH2Channel::readBinaryBlock(softint $size, softint $stream_id = 0, softint $timeout_ms = -1) returns binary
// SSH2Channel::readBinaryBlock(softint $size, softint $stream_id = 0, date $timeout) returns binary
AbstractQoreNode *SSH2CHANNEL_readBinaryBlock(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   static const char *SSH2CHANNEL_READBINARYBLOCK_ERROR = "SSH2CHANNEL-READBINARYBLOCK-ERROR";
   int64 size = HARD_QORE_INT(params, 0);
   if (size <= 0) {
      xsink->raiseException(SSH2CHANNEL_READBINARYBLOCK_ERROR, "expecting a positive size for the block size to read, got %lld instead; use SSH2Channel::readBinary() to read available data without a block size", size);
      return 0;
   }
   int stream = HARD_QORE_INT(params, 1);
   if (stream < 0) {
      xsink->raiseException(SSH2CHANNEL_READBINARYBLOCK_ERROR, "expecting non-negative integer for stream id as optional second argument to SSH2Channel::readBinaryBlock(blocksize, [streamid], [timeout_ms]), got %d instead; use 0 for stdin, 1 for stderr");
      return 0;
   }
   return c->readBinary(size, stream, getMsMinusOneInt(get_param(params, 2)), xsink);
}

// SSHChannel::write(data $data, softint $stream_id = 0, softint $timeout_ms = -1) returns nothing
// SSHChannel::write(data $data, softint $stream_id = 0, date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_write(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   static const char *SSH2CHANNEL_WRITE_ERROR = "SSH2CHANNEL-WRITE-ERROR";
   const void *buf = 0;
   qore_size_t buflen;

   const AbstractQoreNode *p = get_param(params, 0);
   qore_type_t t = p->getType();

   TempEncodingHelper tmp;
   if (t == NT_STRING) {
      const QoreStringNode *str = reinterpret_cast<const QoreStringNode *>(p);
      tmp.set(str, c->getEncoding(), xsink);
      if (*xsink)
	 return 0;

      buf = tmp->getBuffer();
      buflen = tmp->strlen();
   }
   else {
      const BinaryNode *b = reinterpret_cast<const BinaryNode *>(p);
      buf = b->getPtr();
      buflen = b->size();
   }

   // ignore zero-length writes
   if (!buflen)
      return 0;

   int stream = HARD_QORE_INT(params, 1);
   if (stream < 0) {
      xsink->raiseException(SSH2CHANNEL_WRITE_ERROR, "expecting non-negative integer for stream id as optional second argument to SSH2Channel::write(data $data, softint $streamid, $timeout), got %d instead; use 0 for stdin, 1 for stderr");
      return 0;
   }

   c->write(xsink, buf, buflen, stream, getMsMinusOneInt(get_param(params, 2)));
   return 0;
}

// SSH2Channel::close(softint $timeout_ms = -1) returns nothing
// SSH2Channel::close(date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_close(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->close(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::waitClosed(softint $timeout_ms = -1) returns nothing
// SSH2Channel::waitClosed(date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_waitClosed(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->waitClosed(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::getExitStatus() returns int
AbstractQoreNode *SSH2CHANNEL_getExitStatus(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   int rc = c->getExitStatus(xsink);
   return *xsink ? 0 : new QoreBigIntNode(rc);
}

// SSH2Channel::requestX11Forwarding(softint $screen_no = 0, bool $single_connection = False, string $auth_proto = "", string $auth_cookie = "", softint $timeout_ms = -1) returns nothing
// SSH2Channel::requestX11Forwarding(softint $screen_no = 0, bool $single_connection = False, string $auth_proto = "", string $auth_cookie = "", date $timeout) returns nothing
AbstractQoreNode *SSH2CHANNEL_requestX11Forwarding(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   int screen_no = HARD_QORE_INT(params, 0);
   bool single_connection = HARD_QORE_BOOL(params, 1);
   const QoreStringNode *ap = HARD_QORE_STRING(params, 2);
   const QoreStringNode *ac = HARD_QORE_STRING(params, 3);
   c->requestX11Forwarding(xsink, screen_no, single_connection, ap->strlen() ? ap->getBuffer() : 0, ac->strlen() ? ac->getBuffer() : 0, getMsMinusOneInt(get_param(params, 4)));
   return 0;
}

// SSH2Channel::setEncoding(string $encoding) returns nothing
static AbstractQoreNode *SSH2CHANNEL_setEncoding(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   const QoreStringNode *p0 = HARD_QORE_STRING(params, 0);
   c->setEncoding(QEM.findCreate(p0));
   return 0; 
}

// SSH2Channel::getEncoding() returns string
static AbstractQoreNode *SSH2CHANNEL_getEncoding(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   return new QoreStringNode(c->getEncoding()->getCode());
}

// SSH2Channel::extendedDataNormal(softint $timeout_ms = -1) returns nothing
// SSH2Channel::extendedDataNormal(date $timeout) returns nothing
static AbstractQoreNode *SSH2CHANNEL_extendedDataNormal(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->extendedDataNormal(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::extendedDataMerge(softint $timeout_ms = -1) returns nothing
// SSH2Channel::extendedDataMerge(date $timeout) returns nothing
static AbstractQoreNode *SSH2CHANNEL_extendedDataMerge(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->extendedDataMerge(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

// SSH2Channel::extendedDataIgnore(softint $timeout_ms = -1) returns nothing
// SSH2Channel::extendedDataIgnore(date $timeout) returns nothing
static AbstractQoreNode *SSH2CHANNEL_extendedDataIgnore(QoreObject *self, SSH2Channel *c, const QoreListNode *params, ExceptionSink *xsink) {
   c->extendedDataIgnore(xsink, getMsMinusOneInt(get_param(params, 0)));
   return 0;
}

QoreClass *initSSH2ChannelClass() {
   QORE_TRACE("initSSH2Channel()");

   QC_SSH2CHANNEL = new QoreClass("SSH2Channel", QDOM_NETWORK);
   CID_SSH2_CHANNEL = QC_SSH2CHANNEL->getID();

   QC_SSH2CHANNEL->setConstructor(SSH2CHANNEL_constructor);
   QC_SSH2CHANNEL->setCopy((q_copy_t)SSH2CHANNEL_copy);
   QC_SSH2CHANNEL->setDestructor((q_destructor_t)SSH2CHANNEL_destructor);

   // SSH2Channel::setenv(string $var, string $value, softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("setenv",                (q_method_t)SSH2CHANNEL_setenv, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 3, stringTypeInfo, QORE_PARAM_NO_ARG, stringTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::setenv(string $var, string $value, date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("setenv",                (q_method_t)SSH2CHANNEL_setenv, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 3, stringTypeInfo, QORE_PARAM_NO_ARG, stringTypeInfo, QORE_PARAM_NO_ARG, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::requestPty(string $term = "vanilla", string $modes = "", softint $width = LIBSSH2_TERM_WIDTH, softint $height = LIBSSH2_TERM_HEIGHT, softint $width_px = LIBSSH2_TERM_WIDTH_PX, softint $height_px = LIBSSH2_TERM_HEIGHT_PX, softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("requestPty",            (q_method_t)SSH2CHANNEL_requestPty, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 7, stringTypeInfo, new QoreStringNode("vanilla"), stringTypeInfo, new QoreStringNode, softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_WIDTH), softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_HEIGHT), softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_WIDTH_PX), softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_HEIGHT_PX), softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::requestPty(string $term = "vanilla", string $modes = "", softint $width = LIBSSH2_TERM_WIDTH, softint $height = LIBSSH2_TERM_HEIGHT, softint $width_px = LIBSSH2_TERM_WIDTH_PX, softint $height_px = LIBSSH2_TERM_HEIGHT_PX, date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("requestPty",            (q_method_t)SSH2CHANNEL_requestPty, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 7, stringTypeInfo, new QoreStringNode("vanilla"), stringTypeInfo, new QoreStringNode, softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_WIDTH), softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_HEIGHT), softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_WIDTH_PX), softBigIntTypeInfo, new QoreBigIntNode(LIBSSH2_TERM_HEIGHT_PX), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::shell(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("shell",                 (q_method_t)SSH2CHANNEL_shell, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::shell(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("shell",                 (q_method_t)SSH2CHANNEL_shell, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::eof() returns bool
   QC_SSH2CHANNEL->addMethodExtended("eof",                   (q_method_t)SSH2CHANNEL_eof, false, QC_NO_FLAGS, QDOM_DEFAULT, boolTypeInfo);

   // SSH2Channel::sendEof(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("sendEof",               (q_method_t)SSH2CHANNEL_sendEof, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::sendEof(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("sendEof",               (q_method_t)SSH2CHANNEL_sendEof, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::waitEof(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("waitEof",               (q_method_t)SSH2CHANNEL_waitEof, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::waitEof(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("waitEof",               (q_method_t)SSH2CHANNEL_waitEof, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::exec(string $command, softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("exec",                  (q_method_t)SSH2CHANNEL_exec, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::exec(string $command, date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("exec",                  (q_method_t)SSH2CHANNEL_exec, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::subsystem(string $command, softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("subsystem",             (q_method_t)SSH2CHANNEL_subsystem, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::subsystem(string $command, date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("subsystem",             (q_method_t)SSH2CHANNEL_subsystem, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::read(softint $stream_id = 0, softint $timeout_ms = 10000) returns string
   QC_SSH2CHANNEL->addMethodExtended("read",                  (q_method_t)SSH2CHANNEL_read, false, QC_NO_FLAGS, QDOM_DEFAULT, stringTypeInfo, 2, softBigIntTypeInfo, zero(), softBigIntTypeInfo, new QoreBigIntNode(10000));
   // SSH2Channel::read(softint $stream_id = 0, date $timeout) returns string
   QC_SSH2CHANNEL->addMethodExtended("read",                  (q_method_t)SSH2CHANNEL_read, false, QC_NO_FLAGS, QDOM_DEFAULT, stringTypeInfo, 2, softBigIntTypeInfo, zero(), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::readBinary(softint $stream_id = 0, softint $timeout_ms = 10000) returns binary
   QC_SSH2CHANNEL->addMethodExtended("readBinary",            (q_method_t)SSH2CHANNEL_readBinary, false, QC_NO_FLAGS, QDOM_DEFAULT, binaryTypeInfo, 2, softBigIntTypeInfo, zero(), softBigIntTypeInfo, new QoreBigIntNode(10000));
   // SSH2Channel::readBinary(softint $stream_id = 0, date $timeout) returns binary
   QC_SSH2CHANNEL->addMethodExtended("readBinary",            (q_method_t)SSH2CHANNEL_readBinary, false, QC_NO_FLAGS, QDOM_DEFAULT, binaryTypeInfo, 2, softBigIntTypeInfo, zero(), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::readBlock(softint $size, softint $stream_id = 0, softint $timeout_ms = -1) returns string
   QC_SSH2CHANNEL->addMethodExtended("readBlock",             (q_method_t)SSH2CHANNEL_readBlock, false, QC_NO_FLAGS, QDOM_DEFAULT, stringTypeInfo, 3, softBigIntTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, zero(), softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::readBlock(softint $size, softint $stream_id = 0, date $timeout) returns string
   QC_SSH2CHANNEL->addMethodExtended("readBlock",             (q_method_t)SSH2CHANNEL_readBlock, false, QC_NO_FLAGS, QDOM_DEFAULT, stringTypeInfo, 3, softBigIntTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, zero(), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::readBinaryBlock(softint $size, softint $stream_id = 0, softint $timeout_ms = -1) returns binary
   QC_SSH2CHANNEL->addMethodExtended("readBinaryBlock",       (q_method_t)SSH2CHANNEL_readBinaryBlock, false, QC_NO_FLAGS, QDOM_DEFAULT, binaryTypeInfo, 3, softBigIntTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, zero(), softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::readBinaryBlock(softint $size, softint $stream_id = 0, date $timeout) returns binary
   QC_SSH2CHANNEL->addMethodExtended("readBinaryBlock",       (q_method_t)SSH2CHANNEL_readBinaryBlock, false, QC_NO_FLAGS, QDOM_DEFAULT, binaryTypeInfo, 3, softBigIntTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, zero(), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSHChannel::write(data $data, softint $stream_id = 0, softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("write",                 (q_method_t)SSH2CHANNEL_write, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 3, dataTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, zero(), softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSHChannel::write(data $data, softint $stream_id = 0, date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("write",                 (q_method_t)SSH2CHANNEL_write, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 3, dataTypeInfo, QORE_PARAM_NO_ARG, softBigIntTypeInfo, zero(), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::close(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("close",                 (q_method_t)SSH2CHANNEL_close, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::close(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("close",                 (q_method_t)SSH2CHANNEL_close, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::waitClosed(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("waitClosed",            (q_method_t)SSH2CHANNEL_waitClosed, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::waitClosed(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("waitClosed",            (q_method_t)SSH2CHANNEL_waitClosed, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::getExitStatus() returns int
   QC_SSH2CHANNEL->addMethodExtended("getExitStatus",         (q_method_t)SSH2CHANNEL_getExitStatus, false, QC_NO_FLAGS, QDOM_DEFAULT, bigIntTypeInfo);

   // SSH2Channel::requestX11Forwarding(softint $screen_no = 0, bool $single_connection = False, string $auth_proto = "", string $auth_cookie = "", softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("requestX11Forwarding",  (q_method_t)SSH2CHANNEL_requestX11Forwarding, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 5, softBigIntTypeInfo, zero(), boolTypeInfo, &False, stringTypeInfo, null_string(), stringTypeInfo, null_string(), softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::requestX11Forwarding(softint $screen_no = 0, bool $single_connection = False, string $auth_proto = "", string $auth_cookie = "", date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("requestX11Forwarding",  (q_method_t)SSH2CHANNEL_requestX11Forwarding, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 5, softBigIntTypeInfo, zero(), boolTypeInfo, &False, stringTypeInfo, null_string(), stringTypeInfo, null_string(), dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::setEncoding(string $encoding) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("setEncoding",           (q_method_t)SSH2CHANNEL_setEncoding, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, stringTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::getEncoding() returns string
   QC_SSH2CHANNEL->addMethodExtended("getEncoding",           (q_method_t)SSH2CHANNEL_getEncoding, false, QC_RET_VALUE_ONLY, QDOM_DEFAULT, stringTypeInfo);

   // SSH2Channel::extendedDataNormal(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("extendedDataNormal",    (q_method_t)SSH2CHANNEL_extendedDataNormal, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::extendedDataNormal(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("extendedDataNormal",    (q_method_t)SSH2CHANNEL_extendedDataNormal, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::extendedDataMerge(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("extendedDataMerge",     (q_method_t)SSH2CHANNEL_extendedDataMerge, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::extendedDataMerge(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("extendedDataMerge",     (q_method_t)SSH2CHANNEL_extendedDataMerge, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   // SSH2Channel::extendedDataIgnore(softint $timeout_ms = -1) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("extendedDataIgnore",    (q_method_t)SSH2CHANNEL_extendedDataIgnore, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, softBigIntTypeInfo, new QoreBigIntNode(-1));
   // SSH2Channel::extendedDataIgnore(date $timeout) returns nothing
   QC_SSH2CHANNEL->addMethodExtended("extendedDataIgnore",    (q_method_t)SSH2CHANNEL_extendedDataIgnore, false, QC_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, dateTypeInfo, QORE_PARAM_NO_ARG);

   return QC_SSH2CHANNEL;
}
