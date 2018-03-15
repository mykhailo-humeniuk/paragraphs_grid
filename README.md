# paragraphs_grid

This module implements widget and formatter for Paragraphs field.

<h1>Installing</h1>

first of all need to install next modules:
 - Paragraphs - https://www.drupal.org/project/paragraphs
 - jQuery Update - https://www.drupal.org/project/jquery_update
 
On admin/structure/types/manage/[%node_type]/fields page add field type
Paragraph and choose widget Gridstack. ON field settings page set 
<b>Default edit mode</b> as "Preview" and <b>Number of values</b> as "Unlimited".
Also on this page user can choose general settings for Gristack library:
 - alwaysShowResizeHandle - If checked the resizing handles are shown even if the user is not hovering over the widget
 - Float - Enable floating widgets
 - cellHeight - One cell height in pixels
 - height - Maximum rows amount. Default is 0 which means no maximum rows
 - verticalMargin - Vertical gap size in pixels
 - width - Amount of columns. Default 12
 
Also user able to choose individual Gridstack settings for each node, details in 
"Content creation" part.
 
Then, you have to configure jQuery Update module for getting proper version
of jQuery on admin pages for correct work Gridstack library. For this go to page
admin/config/development/jquery_update and choose "1.10" version for 
<b>Alternate jQuery version for administrative pages</b>.


<h1>Content creation</h1>
First step, on node/add page need to choose Gridstack settings for this particular node
in region "GRID SETTINGS" (the same as on field edit page, if user don't want to change
settings for node just need to click on "Save grid" button because of this part
of form will be prefilled with values from general settings which we setted on
field settings page) and click "Save grid" button.
Second step, after clicking on "Save grid" button form with grid setting will be
hidden and Paragraph field will appear.

That's it.
