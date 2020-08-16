# DKAN Export
This module exports data and content from a DKAN Classic site that are not already exposed via API (eg data.json, etc...).

# Usage
## Installation
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
