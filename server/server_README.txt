# Install Ubuntu 16.04 LTS with default username 'boincadm'

# Note: You might want to use newer version of php* packages
sudo apt install m4 make dh-autoreconf pkg-config git vim libapache2-mod-php5 libmysqlclient-dev php5-mysql php5-cli php5-gd python python-mysqldb libssl-dev libcurl4-openssl-dev apache2-utils libboost1.62-all-dev
sudo apt install mysql-server-5.5
# create a mysql root password
sudo apt install phpmyadmin (Configure database with dbconfig-common - YES)
# use mysql root password
# create password for phpmyadmin

sudo addgroup boincadm
sudo usermod -a -G boincadm www-data


# Download the boinc:

cd ~
git clone https://github.com/BOINC/boinc.git boinc-src


# Download the server data:

cd ~
wget https://fitcrack.fit.vutbr.cz/download/server_2018_09_1.tar.gz
tar -xvzf server_2018_09_01.tar.gz


# Copy fitcrack deamons to BOINC directory and install server:
cd server_2018_09_01
./update_server.sh

cd src/patches
./patch_makefile.sh

cd ~/boinc-src
./_autosetup
./configure --disable-client --disable-manager
make
sudo make install

cd ~/boinc-src/tools

# Create the project:
# --url_base is the IP address of your server
# --test_app will be the name of the application,
# --db_user is the mysql username (root)
# --db_passwd is the mysql root password

./make_project --url_base http://*your_server_ip* --db_user root --db_passwd *root_password* --test_app fitcrack

sudo bash -c 'cat /home/boincadm/projects/*your_project_name*/*your_project_name*.httpd.conf >> /etc/apache2/apache2.conf'

sudo vim /etc/apache2/apache2.conf
:%s/Allow from all/Allow from all\r        Require all granted/g
:%s/#ServerRoot "\/etc\/apache2"/#ServerRoot "\/etc\/apache2"\rServerName test.local/g
:wq

sudo /usr/sbin/apache2ctl restart


# Create authentication for BOINC web administration:

htpasswd -cb ~/projects/*your_project_name*/html/ops/.htpasswd *your_username* *your_password*

vim /home/boincadm/projects/*your_project_name*/html/project/project.inc
:%s/<master_url>/http:\/\/*your_server_IP*\//g
:wq

cd /home/boincadm/projects/*your_project_name*/

sudo a2enmod cgi

sudo /usr/sbin/apache2ctl restart


# Edit project config:

vim /home/boincadm/projects/*your_project_name*/config.xml
:%s/    <\/config>/        <enable_assignment>1<\/enable_assignment>\r        <next_rpc_delay>10<\/next_rpc_delay>\r        <maintenance_delay>60<\/maintenance_delay>\r        <max_wus_in_progress>1<\/max_wus_in_progress>\r        <max_ncpus>1<\/max_ncpus>\r        <prefer_primary_platform>1<\/prefer_primary_platform>\r        <min_sendwork_interval>6<\/min_sendwork_interval>\r    <\/config>\r/g
:%s/example_app/fitcrack/g
:%s/sample_work_generator -d 3/work_generator -d 3 --app fitcrack/g
:%s/sample_assimilator/assimilator/g
:%s/sample_bitwise_validator/bitwise_validator/g
:%s/</daemons>/    <daemon>\r            <cmd>trickler --variety fitcrack</cmd>\r        </daemon>\r    </daemons>/g
:wq


# Copy fitcrack to the project:

cd ~
cp server_2018_09_01/templates/* /home/boincadm/projects/*your_project_name*/templates/
cp server_2018_09_01/project.xml /home/boincadm/projects/*your_project_name*/

# Delete default application versions:

rm -r ~/projects/*your_project_name*/apps/example_app/*

# Copy binaries of your host aplication to
# /home/boincadm/project/*your_project_name*/app/fitcrack/*version_number*/
# *version_number* directory has to contain subfolder named as targeted
# platform. All platforme detected by BOINC can 
# be found at https://boinc.berkeley.edu/trac/wiki/BoincPlatforms. Each
# subfolder has to contain all of the files even
# those platform not-specific (archive etc.). Also there has to be version.xml
# with files specified for more info look
# at https://boinc.berkeley.edu/trac/wiki/AppVersionNew.
# When you have files prepared like this you can call ./bin/update_versions
# script which then adds the new version to
# to the system and stages it for sending to clients.

mkdir /home/boincadm/projects/*your_project_name*/apps/fitcrack/1/
cp -r server_2018_09_01/client_bin/* /home/boincadm/projects/*your_project_name*/apps/fitcrack/1/
# import the `server_2018_09_01/sql/*` scripts into MySQL *your_project_name* database

# Note: If phpmyadmin does not work, use this command (works on Ubuntu 16.04):
sudo ln -s /usr/share/phpmyadmin /var/www/html


# Start the server

cd ~/projects/*your_project_name*/

./bin/xadd
./bin/update_versions
./bin/start
./bin/status

# Done. Continue with Web-Admin installation guide to use the installed system through web browser.
