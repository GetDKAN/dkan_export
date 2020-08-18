# DKAN Export
This module exports data and content from a DKAN Classic site that are not already exposed via API (eg data.json, etc...).

# Installation
## Requirements
* Full installation of core DKAN.
* Dependencies:
  * [loft_data_grids](https://www.drupal.org/project/loft_data_grids)

## Install Using Drush Make
```
$ cd sites/all/modules/contrib
$ git clone https://github.com/GetDKAN/dkan_export
$ drush make -y --no-core sites/all/modules/contrib/dkan_export/dkan_export.make
$ drush en dkan_export -y
```

## Install Manually
* Download the zip file from https://github.com/GetDKAN/dkan_export
* Unzip the file in `/sites/all/modules/contrib`
* Download all dependent contrib modules from the Requirements list above and add them to `/sites/all/modules/contrib`
* Enable the module (for example, using drush: `drush en dkan_export -y`)

# Usage
## Export Data
Data exports is made through drush commands that will output the content as CSV unless specified otherwise.

Following the Data exports currently supported:

### Users
Save all users currently available on on a DKAN Classic site and uuids of datasets that they authored.
```
$ drush dkan-export-users > users.csv
```

Additional options:
* --destination-file: Specify a destination file for the output, do not output the result to stdout.
* --only-active: Only include active users, skip blocked users.
* --skip-uuids: Do not include users authored datasets' uuids.
