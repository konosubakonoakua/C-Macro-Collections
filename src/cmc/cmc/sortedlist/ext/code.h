/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#ifdef CMC_EXT_INIT

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * List bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _sort)(target);

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = 0;
    iter.start = true;
    iter.end = CMC_(PFX, _empty)(target);

    return iter;
}

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _sort)(target);

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = 0;
    iter.start = CMC_(PFX, _empty)(target);
    iter.end = true;

    if (!CMC_(PFX, _empty)(target))
        iter.cursor = target->count - 1;

    return iter;
}

bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->start;
}

bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->end;
}

bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = 0;
        iter->start = true;
        iter->end = CMC_(PFX, _empty)(iter->target);

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->start = CMC_(PFX, _empty)(iter->target);
        iter->cursor = iter->target->count - 1;
        iter->end = true;

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->end)
        return false;

    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->cursor++;

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->cursor == 0)
    {
        iter->start = true;
        return false;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->cursor--;

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->end)
        return false;

    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    if (steps == 0 || iter->cursor + steps >= iter->target->count)
        return false;

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->cursor += steps;

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->cursor == 0)
    {
        iter->start = true;
        return false;
    }

    if (steps == 0 || iter->cursor < steps)
        return false;

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->cursor -= steps;

    return true;
}

/* Returns true only if the iterator was able to be positioned at the */
/* given index */
bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (index >= iter->target->count)
        return false;

    if (iter->cursor > index)
        return CMC_(PFX, _iter_rewind)(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return CMC_(PFX, _iter_advance)(iter, index - iter->cursor);

    return true;
}

V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
        return (V){ 0 };

    return iter->target->buffer[iter->cursor];
}

size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return iter->cursor;
}

#endif /* CMC_EXT_ITER */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_list_, FILE *fptr)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *l_ = _list_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%" PRIuMAX ", "
                        "count:%" PRIuMAX ", "
                        "is_sorted:%s, "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(V), l_, l_->buffer, l_->capacity, l_->count,
                        l_->is_sorted ? "true" : "false", l_->flag, l_->f_val, l_->alloc, CMC_CALLBACKS_GET(l_));
}

bool CMC_(PFX, _print)(struct SNAME *_list_, FILE *fptr, const char *start, const char *separator, const char *end)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _sort)(_list_);

    fprintf(fptr, "%s", start);

    for (size_t i = 0; i < _list_->count; i++)
    {
        if (!_list_->f_val->str(fptr, _list_->buffer[i]))
            return false;

        if (i + 1 < _list_->count)
            fprintf(fptr, "%s", separator);
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
