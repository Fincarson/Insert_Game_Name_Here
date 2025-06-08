#ifndef VIDEO_HPP
#define VIDEO_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <string>

#include "Engine/IObject.hpp"

namespace Engine {
    /// <summary>
    /// A simple static image object.
    /// </summary>
    class Video : public IObject {
    public:
        // Smart pointer to bitmap.
        std::shared_ptr<ALLEGRO_BITMAP> bmp;
        /// <summary>
        /// Construct an image object.
        /// </summary>
        /// <param name="img">The image path in 'resources/images/'</param>
        /// <param name="sx">source coordinate begin(X).</param>
        /// <param name="sy">source coordinate begin(Y).</param>
        /// <param name="sw">source width.</param>
        /// <param name="sh">source height.</param>
        /// <param name="dx">game screen coordinate begin(X).</param>
        /// <param name="dy">game screen coordinate begin(Y).</param>
        /// <param name="dw">game screen width.</param>
        /// <param name="dh">game screen height.</param>
        /// <param name="frames">how many frames in the file.</param>
        explicit Video(std::string img, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh, int frames);
        void Update(float deltaTime);
        /// <summary>
        /// Draw the loaded video.
        /// </summary>
        void Draw() const;

    private:
        float sx, sy, sw, sh, dx, dy, dw, dh;
        float cur_sx;
        int frames, tick;
    };
}
#endif