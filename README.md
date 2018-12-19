# Fitcrack (Distributed password cracking system)

## Software prerequisities
* make (3.79+)
* m4 (1.4+)
* libtool (1.5+)
* autoconf (2.58+)
* automake (1.8+)
* GCC (6.3.0+)
* pkg-config (0.15+)
* Python 2 (2.2+)
* Python 3
* pip for Python 3
* MySQL (4.0.9+) or MariaDB (10.0+)
* libnotify-dev
* Apache with the following modules:
  * PHP (5+)
  * CGI
  * WSGI
  * rewrite
* PHP5 with cli support and the GD and MySQL modules
* OpenSSL (0.98+)

## Installation
Create a user for running BOINC server
```
useradd -m -c "BOINC Administrator" boincadm  -s /bin/bash
```
Create a MySQL database and user account for Fitcrack
```
mysql -u root -p
mysql> create database fitcrack;
mysql> GRANT ALL PRIVILEGES ON fitcrack.* TO 'fitcrack'@'localhost' IDENTIFIED BY 'mypassword';
```
As root, use Fitcrack installer:
```
./install_fitcrack.sh
```
Or install everything manually (see README.md files for server, webadmin, and runner).

## Running the server

Login to the server as BOINC user (e.g. `boincadm`) and enter the project directory:
```
boincadm@myserver:~$ cd projects/fitcrack
```

### Get server status
```
boincadm@myserver:~/projects/fitcrack$ ./bin/status
```

### Start Fitcrack server
```
boincadm@myserver:~/projects/fitcrack$ ./bin/start
```

### Stop Fitcrack server
```
boincadm@myserver:~/projects/fitcrack$ ./bin/stop
```

## Step-by-step: Install on Debian 9 / Ubuntu 18.04 LTS

### Install prerequisities
```
sudo apt-get install m4 make dh-autoreconf pkg-config git vim apache2 libapache2-mod-php mysql-server mysql-common libmysqlclient-dev zlibc zlib1g zlib1g-dev php php-mysql php-cli php-gd python python python3 python-mysqldb python3-pymysql python3-pip libapache2-mod-wsgi-py3 libssl-dev libcurl4-openssl-dev apache2-utils libboost1.62-all-dev pkg-config libnotify-dev

mysql_secure_installation # Set MySQL root password

a2enmod cgi       # enable mod CGI
a2enmod rewrite   # enable mod rewrite
a2enmod wsgi      # enable mod wsgi
systemctl apache2 restart
```
### Setup user and Database
```
useradd -m -c "BOINC Administrator" boincadm  -s /bin/bash
mysql -u root -p
mysql> create database fitcrack;
mysql> GRANT ALL PRIVILES ON fitcrack.* TO 'fitcrack'@'localhost' IDENTIFIED BY 'mypassword';
```

### Install Fitcrack
```
./install_fitcrack.sh
```


## Step-by-step: Install on CentOS/RHEL 7

### Install prerequisities
```
yum install centos-release-scl devtoolset-6-gcc* m4 libtool autoconf automake  git vim httpd php php-mysql mod_wsgi mariadb-server mariadb-devel zlib libcurl-devel openssl-libs python rh-python36 rh-python36-mod_wsgi python2-PyMySQL rh-python36-python-PyMySQL boost* pkgconfig libnotify

systemctl start httpd.service
systemctl enable httpd.service
systemctl start mariadb
mysql_secure_installation # Set MariaDB root password
systemctl enable mariadb.service
```

### Setup user and Database
```
useradd -m -c "BOINC Administrator" boincadm  -s /bin/bash
mysql -u root -p
mysql> create database fitcrack;
mysql> GRANT ALL PRIVILES ON fitcrack.* TO 'fitcrack'@'localhost' IDENTIFIED BY 'mypassword';
```

### Switch to GCC6 and Install Fitcrack
```
scl enable devtoolset-6 bash
./install_fitcrack.sh
```


## Optional: multiple workunits per hosts

In Fitcrack, it is possible to assign 2 (or more) workunits per node, and make a node compute a single one, while the others are downloading in backgroud. This reduces the overhead for traffic-extensive attacks - e.g. dictionary ttack with big dictionaries.

To achieve this behavior, in your project config, e.g. `/home/boincadm/fitcrack/config.xml` add in `<boinc><config>` section find the following line:
```
<max_wus_in_progress>1</max_wus_in_progress>
```
and replace it to:
```
<max_wus_in_progress>2</max_wus_in_progress>
```

and restart the server (as BOINC user):
```
boincadm@myserver:~$ cd projects/fitcrack
boincadm@myserver:~/projects/fitcrack$ ./bin/stop
boincadm@myserver:~/projects/fitcrack$ ./bin/start
```

Then on each node, you limit maximum concurrent workunits to 1 by creating `app_config.xml` file in your BOINC client project directory, e.g.:
* `C:\ProgramData\BOINC\projects\myserver_fitcrack` (Windows)
* `/var/lib/boinc-client/projects/myserver_fitcrack/app_config.xml` (Linux)
Please note, that the project directory is created automatically once you add the project to you BOINC client (e.g. via BOINC Manager).

To `app_config.xml` place the following contents:
```
<app_config>
   <app>
      <name>fitcrack</name>
      <max_concurrent>1</max_concurrent>
      <report_results_immediately/>
    </app>
</app_config>
```
