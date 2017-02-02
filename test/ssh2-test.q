#!/usr/bin/env qore
# -*- mode: qore; indent-tabs-mode: nil -*-

%require-our
%requires qore >= 0.8.12
%requires ssh2 >= 1.1

%requires Util

our hash $ehash;
our int $errors = 0;
our int $iters = 1;
our timeout $timeout = 10s;
our hash $o;

const FileContents = "hi there ýčšěáýžšěčéářčě";
const BinContents = binary(FileContents);
const FileLen = strlen(FileContents);
const FileMode = 0755;

const FileNameLen = 40;

const opts = (
    "iters": "i,iters=i",
    "privkey": "k,private-key=s",
    "threads": "t,threads=i",
    "timeout": "T,timeout=i",
    "help": "h,help"
    );

sub get_random_filename() returns string {
    my string $name;

    for (my int $i = 0; $i < FileNameLen; ++$i) {
        my int $c = rand() % 52;
        if ($c < 26)
            $name += chr($c + 65);
        else
            $name += chr($c + 71);
    }

    return $name;
}

sub test_value(any $v1, any $v2, string $msg) {
    if ($v1 === $v2)
        stdout.printf("OK: %s test\n", $msg);
    else {
        stdout.printf("ERROR: %s test failed! (%N != %N)\n", $msg, $v1, $v2);
        ++$errors;
    }
    $ehash.$msg = True;
}

# read channel until a prompt is recognized
sub readUntilPrompt(SSH2Channel $chan) {
    my string $str;
    do {
        # read with a 5 second timeout
        $str = $chan.read(0, 5s);
	stdout.printf("%s", $str);
	stdout.sync();
    } while ($str !~ /[\#\$\>] *$/);
}

sub ssh_test(string $url) {
    my string $file = get_random_filename();
    my string $fn = "/tmp/" + $file;

    my SSH2Client $sc($url);

    if ($o.privkey)
        $sc.setKeys($o.privkey);

    $sc.connect();

    my hash $info = $sc.info();
    stdout.printf("SSH %s@%s:%d auth: %s, hostkey: %n, crypt_cs: %n, tmp: %s\n", $info.ssh2user, $info.ssh2host, $info.ssh2port, $info.authenticated, $info.methods.HOSTKEY, $info.methods.CRYPT_CS, $fn);

    # test SSHClient::scpPut()
    my SSH2Channel $chan = $sc.scpPut($fn, FileLen, 0622, 1982-01-05, 2010-02-01);
    test_value($chan instanceof SSH2Channel, True, "SSH2Client::scpPut()");
    $chan.write(FileContents);
    $chan.close();

    # test readBlock() from SSH2Client::scpGet()
    $chan = $sc.scpGet($fn, -1, \$info);
    test_value($info.size, FileLen, "SSH2Client::scpGet()");
    my string $str = $chan.readBlock($info.size);
    test_value($str, FileContents, "SSH2Channel::readBlock()");

    # test readBinaryBlock() from SSH2Client::scpGet()
    $chan = $sc.scpGet($fn, -1, \$info);
    my binary $b = $chan.readBinaryBlock($info.size);
    test_value($b, BinContents, "SSH2Channel::readBinaryBlock()");

    $chan = $sc.openSessionChannel();
    test_value($chan instanceof SSH2Channel, True, "SSH2Client::openSessionChannel()");

    $chan.exec("ls -l | head 5");
    $str = $chan.read();
    test_value(type($str), Type::String, "SSH2Channel::exec()");

    $chan.sendEof();
    $chan.close();
    my int $rc = $chan.getExitStatus();
    test_value(type($rc), Type::Int, "SSH2Channel::getExitStatus()");

    $chan = $sc.openSessionChannel();
    $chan.requestPty("vt100");
    $chan.shell();

    stdout.printf("=======================================\n");
    stdout.printf("---------- start SSH session ----------\n");
    stdout.printf("=======================================\n");

    readUntilPrompt($chan);
    $chan.write(sprintf("/bin/rm %s\n", $fn));
    readUntilPrompt($chan);
    $chan.write("ls -l | head -5\n");
    readUntilPrompt($chan);

    $chan.sendEof();
    $chan.close();
    stdout.printf("exit\n");
    stdout.printf("=======================================\n");
    stdout.printf("----------- end SSH session -----------\n");
    stdout.printf("=======================================\n");
    stdout.printf("exit status: %d\n", $chan.getExitStatus());
}

sub main() {
    my GetOpt $g(opts);
    $o = $g.parse2(\$ARGV);

    my *string $url = shift $ARGV;
    if (!exists $url || $o.help || $o.iters < 0 || $o.threads < 0) {
	printf("usage: %s <url>
  url examples:
    ssh://user:password@host
 -i,--iters=ARG        iterations per test
 -k,--private-key=ARG  set private key to use for authentication
 -t,--threads=ARG      number of threads
 -T,--timeout=ARG      set timeout in seconds (def: %ds)
 -h,--help             for this help test\n", get_script_name(), $timeout / 1000);
	exit(1);
    }
    srand(now());

    if (!$o.iters)
        $o.iters = 1;
    if (!$o.threads)
        $o.threads = 1;

    if ($o.timeout)
        $timeout = $o.timeout * 1000;

    printf("using libssh2 version: %s\n", SSH2::Version);

    ssh_test($url);
}

main();
