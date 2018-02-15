/**
 * @file
 * Provides GridStack loaders.
 */

(function ($, Drupal, settings) {
  'use strict';
  
  /**
   * Helper function.
   *
   * Add data to json field and send to drupal callback.
   */
  function _saveParagraphPosition(jsonData) {
    const { basePath, pathPrefix } = Drupal.settings;
    const href = `${basePath}${pathPrefix}grid_update`;
    const post = "grid_items=" + JSON.stringify(jsonData);
    let { localStorage } = window;
    localStorage.setItem('gridItems', JSON.stringify(jsonData));
    
    // Send data to drupal side.
    $.ajax({
      url: href,
      type: "POST",
      dataType: 'json',
      data: post,
      success: function (data) {
      }
    });
  }

  /**
   * Implements grid and backbone collections on node edit page.
   */
  Drupal.behaviors.gridstackField = {
    attach: function (context, settings) {
      let $body = $('body');
      let input = '';
      let data = '';
      let $fieldGridstack = $('.form-item-grid');
      let options = {
        alwaysShowResizeHandle: false,
        animate: false,
        cellHeight: '50px',
        verticalMargin: 0,
        width: 12,
        float: false
      };
      let myStorage = window.localStorage;
  
      $fieldGridstack.gridstack(options);

      // Fill in JSON field with parameters from grid items.
      (function () {
        let $grid_container = $('.form-item-grid');
        let $grid_items = $grid_container.find('.grid-stack-item.ui-draggable.ui-resizable');
        let myStorage = window.localStorage;
        let jsonFieldData = (myStorage.getItem('gridItems')) ? JSON.parse(myStorage.getItem('gridItems')) : [];
        
        // Fill in cache if field with json is not empty.
        if (jsonFieldData.length || $grid_items.length) {
          if ($grid_items.length === jsonFieldData.length) {
            _saveParagraphPosition(jsonFieldData);
          }
          else {
            // Synchronize value in kson field and items in grid then save them in cache.
            jsonFieldData = [];
            $grid_items.each(function (key, item) {
              var obj = {
                x: $(item).data('gs-x'),
                y: $(item).data('gs-y'),
                width: $(item).data('gs-width'),
                height: $(item).data('gs-height'),
                delta: $(item).data('delta')
              };
              jsonFieldData.push(obj);
            });
            _saveParagraphPosition(jsonFieldData);
          }
        }

        // Fill in and save data about new item.
        // TODO need to try to refactor this.
        if (context[0] !== undefined && $(context[0].parentNode).hasClass('field-name-field-grid')) {
          var $new_item =  $(context[0].innerHTML).find('.ajax-new-content').closest('.grid-stack-item.ui-draggable.ui-resizable');
          if ($new_item.length) {
            var obj = {
              x: $new_item.data('gs-x'),
              y: $new_item.data('gs-y'),
              width: $new_item.data('gs-width'),
              height: $new_item.data('gs-height'),
              delta: $new_item.data('delta')
            };
            // jsonFieldData.push(obj);
            if (typeof jsonFieldData[$new_item.data('delta')] == 'undefined') {
              jsonFieldData.push(obj);
            }
            _saveParagraphPosition($field, jsonFieldData);
          }
        }

        $('.field-widget-paragraphs-gridstack').once('save-item', function () {
          // Add custom element with value of item height.
          $grid_items.each(function (key, item) {
            var height = $(item).data('gs-height');
            height = 'Height: ' + (height * 50) + 'px';
            $(item).find('.grid-stack-item-content').prepend('<div class="height-counter">' + height + '</div>');
          });

          $(this).on('change', function(event, items) {
            if(items != undefined) {
              $(items).each(function(i) {
                var obj = {
                  x: this.x,
                  y: this.y,
                  width: this.width,
                  height: this.height,
                  delta: this.el[0].dataset.delta
                };

                if ((jsonFieldData.length == 0) || (items[i].el[0].dataset.delta == jsonFieldData.length)) {
                  jsonFieldData.push(obj);
                }
                else {
                  jsonFieldData[this.el[0].dataset.delta].x = this.x;
                  jsonFieldData[this.el[0].dataset.delta].y = this.y;
                  jsonFieldData[this.el[0].dataset.delta].width = this.width;
                  jsonFieldData[this.el[0].dataset.delta].height = this.height;
                }

                // Update custom element with value of item height.
                var height = this.height;
                var $height_container = $(items[i].el[0]).find('.height-counter');
                height = 'Height: ' + (height * 50) + 'px';
                $height_container.text(height);
              });

              _saveParagraphPosition(jsonFieldData);
            }
          });
        });
      })();
    }
  };
})(jQuery, Drupal, Drupal.settings);
